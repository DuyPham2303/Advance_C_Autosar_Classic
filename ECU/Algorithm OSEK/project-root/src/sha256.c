#include "crypto.h"
#include <string.h>

/*
 * ===========================================================================
 *  sha256.c — Cài đặt SHA-256 “tối giản, tự viết” để minh hoạ nguyên lý
 * ===========================================================================
 *
 *  Tham chiếu: FIPS PUB 180-4 (Secure Hash Standard)
 *
 *  GHI CHÚ QUAN TRỌNG:
 *    - Mã này nhằm mục đích học tập / demo. Không tối ưu hiệu năng
 *      và không thêm biện pháp chống rò rỉ kênh kề (timing/power).
 *    - Triển khai theo chuẩn: xử lý từng khối 512 bit (64 byte), mở rộng
 *      lịch thông điệp W[0..63], lặp 64 vòng, đệm (padding) theo quy định
 *      và ghép độ dài thông điệp 64-bit big-endian ở cuối.
 *    - Kích thước độ dài dùng uint64_t → giới hạn tổng độ dài thông điệp
 *      < 2^64 bit. Với size_t và uint64_t thông thường trên máy 64-bit là đủ.
 */

/* Hằng số vòng K[t] của SHA-256 (t = 0..63) theo FIPS 180-4, Sec 4.2.2 */
static const uint32_t K[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

/* Các phép biến đổi bit theo chuẩn:
 *  - ROTR: xoay phải
 *  - SHR : dịch phải logic
 *  - Ch, Maj: hàm lựa chọn và đa số
 *  - Sig0, Sig1: Σ0, Σ1 (các vòng lớn)
 *  - sig0, sig1: σ0, σ1 (mở rộng lịch thông điệp)
 */
#define ROTR(x,n) (((x) >> (n)) | ((x) << (32-(n))))
#define SHR(x,n)  ((x) >> (n))
#define Ch(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define Maj(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define Sig0(x) (ROTR((x),2) ^ ROTR((x),13) ^ ROTR((x),22))
#define Sig1(x) (ROTR((x),6) ^ ROTR((x),11) ^ ROTR((x),25))
#define sig0(x) (ROTR((x),7) ^ ROTR((x),18) ^ SHR((x),3))
#define sig1(x) (ROTR((x),17) ^ ROTR((x),19) ^ SHR((x),10))

/*
 * Xử lý 1 khối 512-bit (64 byte).
 *  - Chuyển 16 từ đầu (big-endian) vào w[0..15].
 *  - Mở rộng lịch thông điệp w[16..63] bằng σ0/σ1.
 *  - Khởi tạo a..h từ H[0..7], lặp 64 vòng:
 *      T1 = h + Σ1(e) + Ch(e,f,g) + K[i] + w[i]
 *      T2 = Σ0(a) + Maj(a,b,c)
 *      cập nhật a..h (dịch chuỗi) và a = T1 + T2
 *  - Cộng kết quả vào vector băm trạng thái H.
 */
static void process(const uint8_t *chunk, uint32_t H[8]) {
    uint32_t w[64];

    /* Tải 16 từ 32-bit đầu vào theo big-endian */
    for (int i = 0; i < 16; ++i) {
        w[i]  = ((uint32_t)chunk[4*i]   << 24)
              | ((uint32_t)chunk[4*i+1] << 16)
              | ((uint32_t)chunk[4*i+2] << 8)
              | ((uint32_t)chunk[4*i+3]);
    }
    /* Mở rộng thành 64 từ: w[i] = σ1(w[i-2]) + w[i-7] + σ0(w[i-15]) + w[i-16] */
    for (int i = 16; i < 64; ++i)
        w[i] = sig1(w[i-2]) + w[i-7] + sig0(w[i-15]) + w[i-16];

    /* Làm việc trên bản sao a..h của H */
    uint32_t a=H[0], b=H[1], c=H[2], d=H[3], e=H[4], f=H[5], g=H[6], h=H[7];

    /* 64 vòng nén */
    for (int i=0;i<64;++i) {
        uint32_t T1 = h + Sig1(e) + Ch(e,f,g) + K[i] + w[i];
        uint32_t T2 = Sig0(a) + Maj(a,b,c);
        h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;
    }

    /* Cộng dồn vào trạng thái băm */
    H[0]+=a; H[1]+=b; H[2]+=c; H[3]+=d; H[4]+=e; H[5]+=f; H[6]+=g; H[7]+=h;
}

/*
 * sha256(data, len, out)
 *  - H[0..7]: giá trị khởi tạo (IV) theo chuẩn.
 *  - Hấp thụ từng khối 64 byte.
 *  - Đệm (padding):
 *      + nối 0x80
 *      + thêm các byte 0 sao cho tổng còn lại của khối là 8 byte cuối
 *      + ghi độ dài thông điệp (tính bằng bit) dạng 64-bit big-endian
 *  - Ghi xuất digest 32 byte big-endian vào out.
 */
void sha256(const uint8_t* data, size_t len, uint8_t out[32]) {
    /* Vector khởi tạo (IV) của SHA-256 */
    uint32_t H[8] = {
        0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
    };

    uint8_t block[64];
    size_t rem = len;

    /* Hấp thụ các khối đầy đủ 64 byte */
    while (rem >= 64) {
        process(data, H);
        data += 64;
        rem  -= 64;
    }

    /* -------- Padding theo FIPS 180-4 --------
     * 1) Nối 0x80
     * 2) Chèn 0 cho đến khi phần còn lại của khối đủ chỗ cho 8 byte độ dài
     * 3) Ghi độ dài (bit) 64-bit big-endian ở cuối khối
     */
    memcpy(block, data, rem);   /* copy phần cuối chưa đủ 64 */
    block[rem++] = 0x80;        /* thêm bit '1' */

    /* Nếu không còn đủ chỗ cho 8 byte độ dài, xử lý khối hiện tại và bắt đầu khối mới */
    if (rem > 56) {
        memset(block+rem, 0, 64-rem);
        process(block, H);
        rem = 0;
    }

    /* Điền 0 cho tới vị trí 56, dành 8 byte cuối để lưu độ dài */
    memset(block+rem, 0, 56-rem);

    /* Độ dài tính theo bit (big-endian 64-bit) */
    uint64_t bitlen = (uint64_t)len * 8u;
    for (int j = 0; j < 8; ++j)
        block[56 + 7 - j] = (uint8_t)(bitlen >> (j*8));

    /* Xử lý khối đệm cuối */
    process(block, H);

    /* Xuất digest 32 byte big-endian */
    for (int j=0;j<8;++j){
        out[4*j]   = (uint8_t)(H[j] >> 24);
        out[4*j+1] = (uint8_t)(H[j] >> 16);
        out[4*j+2] = (uint8_t)(H[j] >> 8);
        out[4*j+3] = (uint8_t)(H[j]);
    }
}
