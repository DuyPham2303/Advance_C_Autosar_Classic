#include "common.h"
#include "aes_cmac.h"
#include <CommonCrypto/CommonCryptor.h>
#include <assert.h>

#define BLK 16  // Kích thước khối AES (128 bit)

/*
 * Rb (const_rb) — hằng số dùng trong bước Generate_Subkey theo NIST SP 800-38B.
 * Với AES-128: Rb = 0x00000000000000000000000000000087 (chỉ byte cuối = 0x87).
 * Khi dịch trái L và bị tràn bit (msb=1), ta XOR với Rb để tạo K1 (tương tự cho K2).
 * Xem NIST SP 800-38B, mục "Subkey Generation". 
 */
static const uint8_t const_rb[BLK] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87
};

/*
 * aes_ecb_encrypt_block()
 * -----------------------
 * "Nguyên tử" mã hoá 1 khối 16 byte bằng AES-128 ở chế độ ECB (không IV, không padding).
 * Đây KHÔNG phải là mã hoá dòng/gói; chỉ là primitive cho CMAC (chuẩn CMAC yêu cầu
 * một "block cipher encryption" E_K() trên đúng 1 khối 128 bit).
 *
 * Trên macOS dùng CommonCrypto:
 *   CCCrypt(kCCEncrypt, kCCAlgorithmAES128, kCCOptionECBMode, ...)
 * - in/out dài 16, không padding.
 * - Trả 0 nếu OK; -1 nếu lỗi thư viện/môi trường.
 *
 * Tham chiếu: manpage CCCrypt (Apple) — kCCOptionECBMode là cờ chọn ECB. 
 */
static int aes_ecb_encrypt_block(const uint8_t key[16], const uint8_t in[16], uint8_t out[16]){
    size_t outlen = 0;
    CCCryptorStatus st = CCCrypt(kCCEncrypt, kCCAlgorithmAES128, kCCOptionECBMode,
                                 key, 16, NULL, in, 16, out, 16, &outlen);
    return (st == kCCSuccess && outlen == 16) ? 0 : -1;
}

/*
 * leftshift_onebit()
 * ------------------
 * Dịch trái 1 bit cho mảng 16 byte (128 bit) — dùng để tính K1, K2.
 * Lưu ý: triển khai kiểu "big-endian bitwise" (bit cao của byte 0 là MSB toàn khối).
 */
static void leftshift_onebit(const uint8_t in[BLK], uint8_t out[BLK]){
    unsigned carry = 0;
    for (int i = BLK - 1; i >= 0; --i) {
        unsigned v = ((unsigned)in[i] << 1) | carry;
        out[i] = (uint8_t)(v & 0xFF);
        carry = (v >> 8) & 1u;  // giữ bit tràn để đưa lên byte cao hơn
    }
}

/*
 * generate_subkeys()
 * ------------------
 * Tạo K1, K2 theo chuẩn CMAC:
 *   L  = AES_K(0^128)
 *   K1 = (L << 1)  nếu MSB(L)=0
 *      = (L << 1) ⊕ Rb nếu MSB(L)=1
 *   K2 = (K1 << 1) nếu MSB(K1)=0
 *      = (K1 << 1) ⊕ Rb nếu MSB(K1)=1
 * Hai subkey này dùng ở khối cuối: XOR với K1 nếu khối cuối đủ 16B; 
 * XOR với K2 nếu khối cuối thiếu và phải pad “10*”.
 */
static void generate_subkeys(const uint8_t key[16], uint8_t K1[BLK], uint8_t K2[BLK]){
    uint8_t L[BLK] = {0};
    uint8_t Z[BLK] = {0};  // 0^128

    //biểu diễn dữ liệu thành ma trận
    (void)aes_ecb_encrypt_block(key, Z, L);

    int msb = (L[0] & 0x80) != 0;
    leftshift_onebit(L, K1);
    if (msb) {
        for (int i = 0; i < BLK; i++) K1[i] ^= const_rb[i];
    }

    msb = (K1[0] & 0x80) != 0;
    leftshift_onebit(K1, K2);
    if (msb) {
        for (int i = 0; i < BLK; i++) K2[i] ^= const_rb[i];
    }
}

/*
 * aes_cmac_128()
 * --------------
 * Tính CMAC/AES-128 theo NIST SP 800-38B / RFC 4493.
 *
 * Thuật toán (tóm tắt):
 * 1) Tạo K1, K2 (như trên).
 * 2) Chia thông điệp M thành khối 16B: M1..Mn (n = ceil(len/16)).
 * 3) Nếu len>0 và len%16==0 (khối cuối đủ):
 *      M_last = M_n ⊕ K1
 *    Ngược lại (không đủ):
 *      M_last = pad_10* (M_n) ⊕ K2   // đệm 0x80 rồi 0x00 đến đủ 16B
 * 4) Tính CBC-MAC sửa lỗi:
 *      X0 = 0^128
 *      Xi = AES_K( Xi-1 ⊕ Mi )    với i = 1..n-1
 *      T  = AES_K( Xn-1 ⊕ M_last )
 *    Kết quả T là thẻ MAC 16B (đầy đủ). Việc cắt ngắn (lấy MSB N bit) làm ở phía người gọi.
 */
int aes_cmac_128(const uint8_t key[16], const uint8_t* msg, size_t len, uint8_t out[16]){
    uint8_t K1[BLK], K2[BLK];
    generate_subkeys(key, K1, K2);

    // Số khối n = ceil(len/BLK). complete=1 nếu khối cuối đủ 16B và len>0.
    size_t n = (len + BLK - 1) / BLK; // ceil
    int complete = (len > 0 && (len % BLK) == 0) ? 1 : 0;

    uint8_t Mlast[BLK];
    if (n == 0) { n = 1; complete = 0; } // Trường hợp len=0: tạo 1 khối giả để pad

    // Chuẩn bị khối cuối cùng (M_last)
    if (complete) {
        // Khối cuối đủ: M_last = M_n XOR K1
        const uint8_t* Mn = msg + (n - 1) * BLK;
        for (int i = 0; i < BLK; i++) Mlast[i] = Mn[i] ^ K1[i];
    } else {
        // Khối cuối thiếu: pad "10*" rồi XOR K2
        uint8_t last[BLK] = {0};
        size_t r = len % BLK;               // số byte còn lại ở khối cuối
        if (r) { memcpy(last, msg + (n - 1) * BLK, r); }
        last[r] = 0x80;                     // đệm bit '1' (0x80), phần còn lại là '0'
        for (int i = 0; i < BLK; i++) Mlast[i] = last[i] ^ K2[i];
    }

    uint8_t X[BLK] = {0}; // X_0 = 0^128
    uint8_t Y[BLK];

    // Xử lý các khối trước khối cuối: Xi = AES_K(Xi-1 ⊕ Mi)
    for (size_t i = 0; i < n - 1; i++) {
        const uint8_t* Mi = msg + i * BLK;
        for (int j = 0; j < BLK; j++) Y[j] = X[j] ^ Mi[j];
        (void)aes_ecb_encrypt_block(key, Y, X);
    }

    // Khối cuối: T = AES_K(X_{n-1} ⊕ M_last)
    for (int j = 0; j < BLK; j++) Y[j] = X[j] ^ Mlast[j];
    (void)aes_ecb_encrypt_block(key, Y, out);
    return 0;
}
