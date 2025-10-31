#ifndef FRESH_H
#define FRESH_H
#include <stdint.h>
#include <stdbool.h>

/*
 * ============================================================================
 *  FRESHNESS MANAGER (TX/RX) — MÔ PHỎNG THEO AUTOSAR SecOC
 *  ----------------------------------------------------------------------------
 *  Mục đích
 *    - Cung cấp bộ đếm Freshness Value (FV) phía GỬI (TX) và logic KHÔI PHỤC
 *      FV_full phía NHẬN (RX) từ phần cắt ngắn (FV_trunc) trong một “cửa sổ
 *      chấp nhận” (acceptance window).
 *
 *  Nguyên tắc chính (tinh thần SecOC):
 *    - FV_full là bộ đếm đơn điệu 64-bit, lưu/khôi phục từ NVM để chống replay.
 *    - Trên bus chỉ mang **LSB(FV_full)** (ví dụ 8 bit) để tiết kiệm băng thông.
 *    - Phía RX tìm ứng viên nhỏ nhất >= (last_ok + 1) khớp LSB(FV_full) trong
 *      phạm vi **RX_ACCEPT_WINDOW**; nếu không khôi phục được → DROP.
 *    - Sau khi xác thực OK, cập nhật **last_ok** = FV_full vừa xác thực.
 *
 *  Liên hệ SecOC:
 *    - SecOC yêu cầu kiểm tra “freshness” của gói (FV) bên cạnh MAC (CMAC/AES-128).
 *    - Nếu truyền FV_full: cũng phải kiểm xem FV nằm trong acceptance window
 *      (tham số kiểu `SecOCRxAcceptanceWindow`).
 *    - Nếu truyền FV_trunc: RX phải dựng “Freshness Verify Value” dựa trên
 *      FV_trunc + trạng thái và thử trong cửa sổ cho phép.
 *
 *  Lưu ý:
 *    - Cần đảm bảo FV_full **không tràn** trong vòng đời khóa hiện hành; khi đổi
 *      khóa, khởi tạo lại counter phù hợp chính sách.
 *    - Hành vi khôi phục dưới đây là mô phỏng SPSC, đơn giản và đủ dùng để test.
 * ============================================================================
 */

/* ----------------------------- RX STATE -------------------------------------
 *  fresh_rx_t giữ:
 *    - last_ok : FV_full đã được xác thực gần nhất.
 *    - window  : kích thước acceptance window (số bước cho phép tìm ứng viên).
 */
typedef struct {
    uint64_t last_ok;   // FV_full đã verify gần nhất (dùng làm mốc khôi phục)
    uint32_t window;    // acceptance window (phạm vi thử FV_full ứng viên)
} fresh_rx_t;

/* ----------------------------- TX STATE -------------------------------------
 *  fresh_tx_t giữ:
 *    - counter : bộ đếm FV_full bên gửi (tăng đơn điệu, lưu NVM).
 */
typedef struct {
    uint64_t counter;   // FV_full của phía gửi (tăng mỗi frame)
} fresh_tx_t;

/**
 * @brief  Khởi tạo Freshness TX (bên gửi) từ giá trị NVM.
 * @param  tx     trạng thái TX
 * @param  start  giá trị khởi tạo FV_full (ví dụ đã đọc từ NVM)
 */
void fresh_tx_init(fresh_tx_t* tx, uint64_t start);

/**
 * @brief  Tăng và trả về FV_full mới để dùng cho frame kế tiếp.
 * @param  tx     trạng thái TX
 * @return FV_full sau khi tăng
 */
uint64_t fresh_tx_next(fresh_tx_t* tx);  // increments and returns new FV_full

/**
 * @brief  Lấy LSB của FV_full để mang lên bus (truncation).
 * @param  fv_full  FV 64-bit đầy đủ
 * @param  bits     số bit LSB cần cắt (ví dụ 8)
 * @return byte chứa LSB đã cắt (nếu bits <= 8), dùng làm FV_trunc
 *
 * @note   Ở hồ sơ phổ biến trên CAN/CAN-FD, thường gửi 8 bit LSB.
 */
uint8_t  fresh_trunc_lsbits(uint64_t fv_full, unsigned bits);

/**
 * @brief  Khởi tạo Freshness RX (bên nhận): mốc last_ok & window.
 * @param  rx      trạng thái RX
 * @param  start   mốc last_ok (ví dụ giá trị đã verify trước đó hoặc NVM)
 * @param  window  kích thước acceptance window (số bước thử tối đa)
 */
void fresh_rx_init(fresh_rx_t* rx, uint64_t start, uint32_t window);

/**
 * @brief  Khôi phục FV_full tối thiểu >= (last_ok + 1) khớp LSB = trunc,
 *         tìm trong khoảng [last_ok+1, last_ok+window].
 *
 * @param  rx        trạng thái RX (chứa last_ok, window)
 * @param  trunc     giá trị FV_trunc nhận trên bus (LSB của FV_full)
 * @param  bits      số bit LSB đã cắt khi truyền (ví dụ 8)
 * @param  out_full  (out) nhận FV_full ứng viên nếu tìm được
 * @return true nếu khôi phục được; false nếu vượt cửa sổ / không khớp
 *
 * @note   Đây là bước dựng “Freshness Verify Value” để đưa vào tính MAC.
 *         Nếu fail → coi như “non-fresh” và DROP gói.
 */
bool fresh_reconstruct_full(const fresh_rx_t* rx, uint8_t trunc, unsigned bits, uint64_t* out_full);

/**
 * @brief  Cập nhật mốc last_ok sau khi verify thành công.
 * @param  rx       trạng thái RX
 * @param  fv_full  FV_full vừa xác thực (được phép tăng mốc)
 */
void fresh_rx_commit(fresh_rx_t* rx, uint64_t fv_full);

#endif
