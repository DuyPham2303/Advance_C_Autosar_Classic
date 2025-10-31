#ifndef FRAMES_H
#define FRAMES_H
#include <stdint.h>

/*
 * ============================================================================
 *  FRAMES: Định nghĩa khung "Secured I-PDU" 8 byte cho mô phỏng SecOC
 *  ----------------------------------------------------------------------------
 *  Mục đích
 *    - Chuẩn hoá layout khung bảo vệ theo tinh thần AUTOSAR SecOC:
 *        [DataID || Payload || FV_full]  -->  Tính CMAC/AES-128 (lấy MSB cắt ngắn)
 *      và mang lên bus dạng "Secured I-PDU" với FV_trunc + MAC_trunc.
 *
 *  Layout 8 byte (giản lược, phù hợp CAN cổ điển 8B hoặc CAN-FD dùng khung nhỏ):
 *      [DataID(2) | Speed(2) | FV_trunc(1) | MAC_trunc(3)]
 *
 *  Ghi chú
 *    - DataID: ràng dữ liệu/mac với đúng PDU (tránh “cross-PDU reuse”).
 *    - Speed:  ví dụ payload 16-bit (đơn vị 0.1 km/h); tuỳ ứng dụng mà thay.
 *    - FV_trunc: LSB của Freshness Value (ví dụ 8 bit) để bên nhận khôi phục FV_full.
 *    - MAC_trunc: MSB của CMAC/AES-128 (ví dụ 24 bit) để xác thực nhanh gọn.
 *    - Endianness "trên dây": big-endian cho các trường 16-bit (nhất quán với helpers).
 *
 *  Mở rộng
 *    - Nếu muốn mô phỏng CAN-FD payload dài (tới 64B), có thể định nghĩa khung lớn
 *      hơn và giữ nguyên nguyên tắc: payload + FV_full (để tính MAC) + mang FV_trunc,
 *      MAC_trunc trên bus.
 * ============================================================================
 */

// Khung Secured I-PDU 8 byte: [DataID 2][Speed 2][FV_trunc 1][MAC_trunc 3]
typedef struct {
    uint16_t data_id;     // Mã PDU (2B, big-endian khi encode ra dây)
    uint16_t speed;       // Ví dụ payload: tốc độ (2B, đơn vị 0.1 km/h)
    uint8_t  fv_trunc;    // LSB(FV_full), ví dụ 8 bit mang lên bus
    uint8_t  mac_trunc[3];// MSB(CMAC_AES_128(...)), ví dụ 24 bit
} frame8_t;

/**
 * @brief  Mã hoá struct frame8_t thành 8 byte theo layout cố định.
 *         - DataID và Speed được encode big-endian.
 *         - fv_trunc và mac_trunc chép thẳng.
 *
 * @param  out  Mảng 8 byte đầu ra (đã cấp sẵn).
 * @param  f    Con trỏ tới frame nguồn.
 */
void frame8_encode(uint8_t out[8], const frame8_t* f);

/**
 * @brief  Giải mã mảng 8 byte thành struct frame8_t theo layout cố định.
 *         - DataID và Speed được decode big-endian.
 *         - fv_trunc và mac_trunc chép thẳng.
 *
 * @param  f    Con trỏ tới frame đích (điền dữ liệu).
 * @param  raw  Mảng 8 byte nguồn từ bus.
 */
void frame8_decode(frame8_t* f, const uint8_t raw[8]);

#endif
