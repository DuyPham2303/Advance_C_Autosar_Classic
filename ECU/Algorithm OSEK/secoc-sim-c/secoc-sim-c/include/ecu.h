#ifndef ECU_H
#define ECU_H
#include "secoc.h"
#include "bus.h"

/*
 * ============================================================================
 *  ECU WRAPPER (A ↔ B) CHO MÔ PHỎNG SecOC TRÊN "BUS" GIẢ LẬP
 *  ----------------------------------------------------------------------------
 *  Mục đích
 *    - Đóng gói ngữ cảnh SecOC (khóa, DataID, cấu hình cắt ngắn, Freshness)
 *      cùng với tên ECU để log rõ ràng khi truyền/nhận.
 *    - Cung cấp API mức “ứng dụng ECU” để:
 *        + GỬI 1 khung đã bảo vệ bởi SecOC (tính CMAC, gắn FV_trunc, đóng gói).
 *        + NHẬN & XỬ LÝ 1 khung từ bus (khôi phục FV_full, verify CMAC, log).
 *
 *  Kiến trúc & dòng chảy
 *    - Phía gửi: App → SecOC (tính CMAC/AES-128 trên [DataID||Payload||FV_full])
 *      → đóng gói frame 8 byte → đẩy vào bus (ring buffer).
 *    - Phía nhận: kéo 1 frame từ bus → SecOC (reconstruct FV_full từ FV_trunc
 *      trong cửa sổ chấp nhận; tính lại CMAC; so sánh MSB MAC_trunc) → kết luận.
 *
 *  Mô hình thời gian/chạy thử
 *    - `ecu_send_speed()` là hành động “producer” (xếp 1 frame vào bus).
 *    - `ecu_process_once()` là hành động “consumer” (rút 1 frame nếu có).
 *    - Triển khai bus hiện tại không khóa; phù hợp mô phỏng tuần tự/SPSC.
 *
 *  Mở rộng
 *    - Có thể bổ sung nhiều API gửi các PDU khác (với DataID khác).
 *    - Có thể thay frame 8B bằng 64B để mô phỏng CAN-FD đầy đủ.
 * ============================================================================
 */

// Mỗi ECU có tên (để log) và 1 ngữ cảnh SecOC (khóa, DataID, cấu hình, Freshness).
typedef struct {
    const char* name;   // Nhãn hiển thị khi log (ví dụ: "ECU-A", "ECU-B")
    secoc_ctx_t secoc;  // Ngữ cảnh SecOC (khóa, DataID, MAC/FV trunc, Freshness TX/RX)
} ecu_t;

/**
 * @brief  Gửi 1 khung "Speed" từ ECU 'a' vào bus (vai trò *sender*).
 *
 * Quy trình bên trong:
 *   1) Lấy FV_full mới từ Freshness TX của 'a' (tăng bộ đếm).
 *   2) Tính CMAC/AES-128 trên [DataID||Speed||FV_full], cắt MAC theo cấu hình.
 *   3) Đóng gói frame 8B: [DataID(2) | Speed(2) | FV_trunc(1) | MAC_trunc(3)].
 *   4) Đẩy vào bus (ring buffer). Nếu đầy → báo drop.
 *
 * @param a      con trỏ tới ECU (đã cấu hình secoc.ftx, key, DataID… cho chiều gửi)
 * @param bus    con trỏ tới bus giả lập
 * @param speed  giá trị tốc độ (ví dụ đơn vị 0.1 km/h) đóng vai trò payload
 */
void ecu_send_speed(ecu_t* a, bus_t* bus, uint16_t speed);

/**
 * @brief  Xử lý nhận 1 khung cho ECU 'b' (vai trò *receiver*).
 *
 * Quy trình bên trong:
 *   1) Rút 1 frame 8B từ bus; nếu rỗng → return ngay.
 *   2) Giải mã trường [DataID|Speed|FV_trunc|MAC_trunc].
 *   3) Từ FV_trunc + last_ok + cửa sổ chấp nhận → khôi phục FV_full (Freshness RX).
 *   4) Tính lại CMAC/AES-128 và so sánh MAC_trunc theo kiểu hằng-thời-gian.
 *   5) Nếu hợp lệ → cập nhật last_ok và log "ACCEPT" (+ FV_full); ngược lại "DROP".
 *
 * @param b       con trỏ tới ECU (đã cấu hình secoc.frx, key, DataID… cho chiều nhận)
 * @param bus     con trỏ tới bus giả lập
 */
void ecu_process_once(ecu_t* b, bus_t* bus);

#endif
