#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

/*
 * ============================================================================
 *  COMMON: Tiện ích chung cho mã hoá khung & debug
 *  ----------------------------------------------------------------------------
 *  Mục đích
 *    - Cung cấp các hàm trợ giúp thao tác số nguyên theo định dạng
 *      **big-endian (MSB-first)** khi đưa ra "trên dây" (wire format),
 *      phù hợp thông lệ **network byte order** trong nhiều giao thức.
 *    - Hàm XOR khối để thao tác mảng byte (hữu ích cho MAC, CMAC...).
 *    - Hàm in hexdump gọn phục vụ debug/log.
 *
 *  Ghi chú chung
 *    - Các hàm beXXenc()/beXXdec() ở đây **tự hiện thực** (không phụ thuộc
 *      vào <endian.h>), vì thế dùng được ở môi trường tối giản và nhất quán
 *      giữa macOS/Linux.
 *    - Giá trị trả về/đầu vào kiểu số là **host-endian**; còn mảng byte đầu ra/
 *      đầu vào được **mã hoá/giải mã theo big-endian**.
 *    - print_hex() chỉ dành cho debug; không dùng để in dữ liệu bí mật ở hệ
 *      thống thực (tránh lộ thông tin).
 * ============================================================================
 */

/* ---------------------------------------------------------------------------
 *  be16enc
 *  --------------------------------------------------------------------------
 *  Mã hoá số 16-bit (host-endian) thành 2 byte **big-endian** vào bộ đệm đích.
 *
 *  dst[0] = byte có trọng số cao (MSB), dst[1] = byte có trọng số thấp (LSB).
 *  Dùng cho việc đóng gói trường 16-bit ra "wire format".
 */
static inline void be16enc(uint8_t *dst, uint16_t v){
    dst[0] = (uint8_t)(v >> 8);
    dst[1] = (uint8_t)(v & 0xFF);
}

/* ---------------------------------------------------------------------------
 *  be64enc
 *  --------------------------------------------------------------------------
 *  Mã hoá số 64-bit (host-endian) thành 8 byte **big-endian**.
 *
 *  Vòng lặp đi từ bit cao → thấp để đảm bảo thứ tự MSB trước (chuẩn BE).
 *  Lưu ý: không phụ thuộc canh hàng (alignment) vì thao tác từng byte.
 */
static inline void be64enc(uint8_t *dst, uint64_t v){
    for(int i=7;i>=0;--i){ dst[7-i] = (uint8_t)(v >> (i*8)); }
}

/* ---------------------------------------------------------------------------
 *  be16dec
 *  --------------------------------------------------------------------------
 *  Giải mã mảng 2 byte **big-endian** về số 16-bit (host-endian).
 *
 *  p[0] là MSB, p[1] là LSB. Trả về giá trị 16-bit tương ứng.
 */
static inline uint16_t be16dec(const uint8_t *p){
    return (uint16_t)p[0]<<8 | p[1];
}

/* ---------------------------------------------------------------------------
 *  xor_block
 *  --------------------------------------------------------------------------
 *  Thực hiện phép XOR từng byte: r[i] = a[i] ^ b[i] với i = 0..n-1.
 *
 *  - Không cấp phát động; chạy O(n).
 *  - Thường dùng trong các bước tính MAC (ví dụ CMAC: X ⊕ M).
 *  - Yêu cầu: r, a, b phải trỏ tới vùng nhớ hợp lệ tối thiểu n byte.
 */
static inline void xor_block(uint8_t *r, const uint8_t *a, const uint8_t *b, size_t n){
    for(size_t i=0;i<n;++i) r[i] = a[i]^b[i];
}

/* ---------------------------------------------------------------------------
 *  print_hex
 *  --------------------------------------------------------------------------
 *  In mảng byte dưới dạng hex in hoa, mỗi byte 2 chữ số (ví dụ "0A1F...").
 *
 *  - tag != NULL: in nhãn trước (không xuống dòng).
 *  - Dùng "%02X" để luôn đủ 2 ký tự và in hoa (A..F).
 *  - Chỉ dùng cho debug/log; tránh in dữ liệu nhạy cảm.
 */
static inline void print_hex(const char* tag, const uint8_t* b, size_t n){
    if(tag) fprintf(stdout, "%s", tag);
    for(size_t i=0;i<n;++i) fprintf(stdout, "%02X", b[i]);
    fputc('\n', stdout);
}

#endif
