#ifndef AES_CMAC_H
#define AES_CMAC_H

#include <stdint.h>
#include <stddef.h>

/*
 * ============================================================================
 *  AES-128 CMAC — Giao diện tính MAC theo NIST SP 800-38B & RFC 4493
 *  ----------------------------------------------------------------------------
 *  Mục đích:
 *    - Tính mã xác thực thông điệp (Message Authentication Code — MAC) bằng
 *      CMAC dựa trên khối mã AES-128 để đảm bảo TÍNH XÁC THỰC & TOÀN VẸN.
 *    - KHÔNG cung cấp bảo mật nội dung (không mã hóa). Nếu cần mật mã hóa,
 *      dùng thêm AEAD (AES-CCM/GCM) hoặc lớp vận chuyển bảo mật.
 *
 *  Chuẩn tham chiếu:
 *    - NIST SP 800-38B: đặc tả đầy đủ CMAC (subkey K1/K2, padding 10*, khối cuối).
 *    - RFC 4493: đặc tả tiện dụng cho AES-CMAC, kèm test vectors chuẩn.
 *
 *  Hành vi/Quy ước quan trọng:
 *    - Khóa đối xứng AES có độ dài 16 byte (AES-128). Khối AES luôn 128 bit.
 *    - Hàm trả về thẻ MAC đầy đủ 16 byte (T). Khi hệ thống cần cắt ngắn
 *      (truncation), HÃY cắt ở mức gọi hàm (ví dụ lấy MSB 24/32/64 bit…).
 *      Theo chuẩn, độ dài MAC sử dụng (Tlen) phải CỐ ĐỊNH cho mọi lần gọi
 *      với cùng 1 khóa trong suốt vòng đời khóa để tránh suy yếu an ninh.
 *    - So sánh MAC khi kiểm tra phải theo kiểu hằng-thời gian (constant-time).
 *    - Không dùng lại cùng 1 khóa cho “mã hóa dữ liệu” và “tính MAC”.
 *
 *  Tính đúng thuật toán:
 *    - Generate_Subkey: L = AES_K(0^128), suy ra K1,K2 (dịch trái 1 bit,
 *      nếu tràn thì XOR với Rb=0x87 ở byte cuối).
 *    - Chia khối 128 bit; khối cuối đủ/không đủ thì XOR với K1/K2 tương ứng,
 *      khối không đủ đệm “10*”.
 *    - Tính chuỗi CBC-MAC sửa lỗi: X_0=0; X_i = AES_K(X_{i-1} XOR M_i);
 *      với M_last đã xử lý như trên; đầu ra cuối cùng là T = X_n (16B).
 *
 *  Tính tương thích & nền tảng:
 *    - Phần hiện thực tham chiếu cần một primitive “AES-ECB mã hóa 1 khối”.
 *      Trên macOS có thể dùng CommonCrypto (CCCrypt với kCCOptionECBMode).
 *      Trên các nền khác có thể dùng OpenSSL EVP hoặc thư viện AES tương đương.
 *
 *  Thí dụ sử dụng:
 *      uint8_t K[16]   = { ... };             // khóa 128 bit
 *      uint8_t tag[16] = {0};
 *      const uint8_t msg[] = "hello";
 *      int rc = aes_cmac_128(K, msg, sizeof(msg)-1, tag);
 *      // Nếu cần MAC 24 bit cho CAN/SecOC: dùng 3 byte MSB của 'tag'
 *
 *  Trả về:
 *      0  : thành công, 'out' nhận 16 byte MAC đầy đủ.
 *     <0  : lỗi nội bộ (ví dụ: primitive AES thất bại).
 *
 *  Bảo trì:
 *    - Hàm là “pure function” theo tham số truyền vào; an toàn luồng nếu
 *      AES primitive bên dưới an toàn luồng.
 *    - Không cấp phát động; không phụ thuộc endian (xử lý theo mảng byte).
 * ============================================================================
 */

/**
 * @brief  Tính AES-CMAC (AES-128) cho thông điệp tùy độ dài.
 *
 * @param  key   Mảng 16 byte — khóa đối xứng AES-128.
 * @param  msg   Con trỏ tới dữ liệu thông điệp.
 * @param  len   Độ dài thông điệp (byte).
 * @param  out   Mảng 16 byte nhận MAC đầy đủ (MSB trước). Có thể cắt ngắn ở ngoài.
 *
 * @return 0 nếu thành công; giá trị âm nếu lỗi.
 *
 * @note   Nếu hệ thống yêu cầu MAC cắt ngắn (ví dụ 24/32/64 bit), luôn
 *         giữ cố định số bit cắt ngắn cho cùng một khóa trong toàn bộ
 *         vòng đời khóa để phù hợp khuyến nghị trong NIST SP 800-38B.
 */
int aes_cmac_128(const uint8_t key[16], const uint8_t* msg, size_t len, uint8_t out[16]);

#endif /* AES_CMAC_H */
