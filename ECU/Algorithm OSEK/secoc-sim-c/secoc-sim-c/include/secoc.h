#ifndef SECOC_H
#define SECOC_H
#include <stdint.h>
#include <stdbool.h>
#include "frames.h"
#include "fresh.h"

/*
 * ============================================================================
 *  SECOC CONTEXT & API — Bảo vệ PDU theo tinh thần AUTOSAR SecOC
 *  ----------------------------------------------------------------------------
 *  Mục tiêu
 *    - Tính MAC bằng CMAC/AES-128 trên chuỗi chuẩn:
 *        Authentic-Data = DataID || Payload || FV_full
 *      rồi mang lên bus “Secured I-PDU” gồm payload + FV_trunc + MAC_trunc.
 *    - Đảm bảo TÍNH XÁC THỰC & TOÀN VẸN dữ liệu. (SecOC **không mã hóa** nội dung.)
 *
 *  Freshness
 *    - FV_full (64-bit) tăng đơn điệu ở phía gửi (TX) và lưu/khôi phục từ NVM.
 *    - Trên bus chỉ gửi LSB(FV_full) = FV_trunc (ví dụ 8 bit) để tiết kiệm chỗ.
 *    - Phía nhận (RX) sẽ khôi phục FV_full tối thiểu ≥ (last_ok+1) khớp LSB,
 *      tìm trong “acceptance window”; nếu không tìm được ⇒ DROP (không xác thực).
 *
 *  Truncation
 *    - MAC_trunc: lấy **MSB** của CMAC (ví dụ 24/32/64 bit...). Độ dài MAC sử dụng
 *      nên cố định cho suốt vòng đời khóa (khuyến nghị từ chuẩn CMAC).
 *    - FV_trunc: số bit LSB của FV_full được gửi kèm (ví dụ 8 bit).
 *
 *  Lưu ý an toàn
 *    - So sánh MAC theo kiểu “constant-time”. (Phần verify trong .c đã làm gọn.)
 *    - Không dùng lại cùng một khóa cho nhiều mục đích (mã hóa & MAC).
 *    - DataID nên được đưa vào Authentic-Data để ràng PDU (tránh cross-PDU reuse).
 *
 *  Vai trò TX/RX
 *    - ctx.ftx != NULL → dùng cho **gửi** (build frame).
 *    - ctx.frx != NULL → dùng cho **nhận** (verify frame).
 * ============================================================================
 */

/* Ngữ cảnh SecOC cho một chiều PDU (ví dụ A→B hoặc B→A) */
typedef struct {
    const uint8_t* key;      // Khóa đối xứng AES-128 (16 byte)
    uint16_t data_id;        // Mã PDU để ràng buộc vào MAC
    unsigned mac_trunc_bits; // Số bit MAC cắt ngắn (lấy MSB của CMAC)
    unsigned fv_trunc_bits;  // Số bit FV cắt ngắn (lấy LSB của FV_full)
    fresh_tx_t* ftx;         // Quản lý Freshness phía gửi  (NULL nếu chỉ làm RX)
    fresh_rx_t* frx;         // Quản lý Freshness phía nhận (NULL nếu chỉ làm TX)
} secoc_ctx_t;

/**
 * @brief  Xây dựng 1 khung Secured I-PDU (8B) từ giá trị payload `speed`.
 *
 * Quy trình bên trong (rút gọn theo SecOC):
 *   1) Lấy FV_full mới từ ftx (tăng đếm) → fv_trunc = LSB(FV_full).
 *   2) Authentic-Data = data_id || speed || FV_full (đúng thứ tự & endianness).
 *   3) Tính CMAC/AES-128(key, Authentic-Data) → lấy MSB mac_trunc_bits.
 *   4) Gói thành frame8_t: [DataID2 | Speed2 | FV_trunc1 | MAC_trunc3] và trả ra `out`.
 *
 * Yêu cầu:
 *   - ctx.ftx phải khác NULL (đang đóng vai “sender” cho chiều này).
 *   - Các tham số truncation được cấu hình hợp lệ (ví dụ 24b MAC, 8b FV).
 */
void secoc_build_frame(secoc_ctx_t* ctx, uint16_t speed, frame8_t* out);

/**
 * @brief  Xác thực 1 khung Secured I-PDU “đến” và (nếu hợp lệ) cập nhật Freshness.
 *
 * Quy trình bên trong (rút gọn theo SecOC):
 *   1) Dựa vào fv_trunc của khung + last_ok + window → khôi phục FV_full ứng viên
 *      nhỏ nhất ≥ (last_ok+1) khớp LSB(FV_full). Nếu vượt cửa sổ → trả false.
 *   2) Dựng lại Authentic-Data = data_id || speed || FV_full(ứng viên).
 *   3) Tính lại CMAC/AES-128 và so sánh MAC_trunc theo constant-time.
 *   4) Nếu khớp → commit last_ok = FV_full, ghi ra *out_fv_full (nếu != NULL) và true.
 *      Ngược lại → false (DROP).
 *
 * Yêu cầu:
 *   - ctx.frx phải khác NULL (đang đóng vai “receiver” cho chiều này).
 */
bool secoc_verify_frame(secoc_ctx_t* ctx, const frame8_t* in, uint64_t* out_fv_full);

#endif
