#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/*
 * ============================================================================
 *  CẤU HÌNH MÔ PHỎNG SecOC (CMAC/AES-128) CHO HAI ECU A↔B
 *  ----------------------------------------------------------------------------
 *  Nguyên tắc (theo AUTOSAR SecOC):
 *    - Tính MAC bằng CMAC/AES-128 trên (DataID || Payload || FV_full).
 *    - Truyền kèm MAC đã cắt ngắn (tiết kiệm băng thông) và FV_trunc (LSB của FV_full).
 *    - Bên nhận dùng FV_trunc + trạng thái để khôi phục FV_full trong "cửa sổ chấp nhận",
 *      sau đó tính lại CMAC và so sánh dạng constant-time.
 *
 *  Lưu ý:
 *    - SecOC chỉ đảm bảo tính xác thực/toàn vẹn (không mã hóa nội dung).
 *    - MAC/FV nên cố định độ dài cắt ngắn cho suốt vòng đời của cùng một khóa.
 *    - FV_full là bộ đếm đơn điệu, lưu NVM để chống replay xuyên phiên nguồn.
 *
 *  Tài liệu gốc:
 *    - AUTOSAR SWS/PRS Secure Onboard Communication (SecOC) — CMAC/AES-128, FV & truncation.
 *    - NIST SP 800-38B, RFC 4493 — đặc tả CMAC/AES-128 (K1/K2, padding 10*).
 * ============================================================================
 */

// === Mã PDU (Data ID) cho hai chiều kênh bảo vệ ===
//  - DATA_ID_A2B: PDU từ ECU A → ECU B.
//  - DATA_ID_B2A: PDU từ ECU B → ECU A (ví dụ dùng làm ACK/Status).
//  Gợi ý: ràng buộc MAC với đúng PDU bằng cách đưa DataID vào vùng tính CMAC.
#define DATA_ID_A2B  0x1234u
#define DATA_ID_B2A  0x2345u

// === Độ dài cắt ngắn (truncation sizes) ===
//  - MAC_TRUNC_BITS: số bit cao (MSB) của CMAC mang lên bus (trade-off an ninh/băng thông).
//      * 24 bit là cấu hình phổ biến trên CAN/CAN-FD khi payload chật (SecOC Profile 1).
//  - FV_TRUNC_BITS: số bit thấp (LSB) của Freshness Value được gửi kèm.
//      * 8 bit thường dùng; bên nhận sẽ tái dựng FV_full trong "acceptance window".
#define MAC_TRUNC_BITS   24u   // lấy MSB 24 bit của CMAC
#define FV_TRUNC_BITS     8u   // gửi LSB 8 bit của FV_full

// === Acceptance window cho phía nhận ===
//  - RX_ACCEPT_WINDOW: số bước đếm FV_full tối đa mà phía nhận sẽ thử để khôi phục
//    FV_full từ FV_trunc (tính từ last_ok+1 đến last_ok+window).
//  - Nếu lệch vượt cửa sổ (mất đồng bộ lâu), cần cơ chế đồng bộ lại (ví dụ gửi Sync).
#define RX_ACCEPT_WINDOW  16u  // điều chỉnh theo jitter/mất gói trong mô phỏng

// === Giá trị FV khởi tạo (mô phỏng đọc lại từ NVM) ===
//  - FV_START_A: giá trị bộ đếm FV_full của kênh A→B khi khởi động (trước khi tăng).
//  - FV_START_B: tương tự cho kênh B→A.
//  - Quy ước điển hình: khi đổi khóa, đặt counter về giá trị nhỏ (không tràn trong vòng đời khóa).
#define FV_START_A   0x0000000000001000ULL
#define FV_START_B   0x0000000000002000ULL

// === Khóa đối xứng ví dụ (16 byte) — CHỈ DÙNG CHO DEMO ===
//  - Mỗi chiều kênh dùng một khóa riêng (tránh reuse cùng khóa cho nhiều mục đích).
//  - Trong sản phẩm thực, khóa phân phối qua HSM/KeyMgr; KHÔNG hardcode như ví dụ.
static const uint8_t SEC_KEY_A2B[16] = {
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF
};
static const uint8_t SEC_KEY_B2A[16] = {
    0x0F,0x1E,0x2D,0x3C,0x4B,0x5A,0x69,0x78,0x87,0x96,0xA5,0xB4,0xC3,0xD2,0xE1,0xF0
};

#endif
