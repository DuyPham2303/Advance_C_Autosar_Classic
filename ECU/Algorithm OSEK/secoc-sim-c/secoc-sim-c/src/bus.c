#include "bus.h"
#include <string.h>

/*
 * ============================================================================
 *  BUS (RING BUFFER 8-BYTE FRAMES)
 *  ----------------------------------------------------------------------------
 *  Mục đích:
 *    - Mô phỏng một “bus” dạng hàng đợi vòng (circular/ring buffer) lưu các
 *      khung cố định 8 byte (giản lược theo CAN cổ điển; có thể mở rộng cho
 *      CAN-FD nếu cần).
 *
 *  Cấu trúc & Quy ước:
 *    - data[64][8]: bộ đệm 64 vị trí, mỗi vị trí là 1 frame 8 byte.
 *    - head: chỉ số ghi kế tiếp (producer sẽ ghi vào đây).
 *    - tail: chỉ số đọc kế tiếp (consumer sẽ đọc ở đây).
 *    - count: số phần tử hiện có trong hàng đợi (0..64).
 *      * RỖNG  khi count == 0  → không có gì để nhận.
 *      * ĐẦY   khi count == 64 → không thêm khung mới (không ghi đè).
 *    - head, tail “quấn vòng” (wrap-around) bằng toán học modulo 64.
 *
 *  Độ phức tạp:
 *    - Gửi (enqueue) & Nhận (dequeue) đều O(1), không cấp phát động.
 *
 *  Ghi chú đồng bộ/đa luồng:
 *    - Triển khai này KHÔNG dùng khoá; phù hợp mô phỏng tuần tự hoặc SPSC
 *      (single-producer/single-consumer) trong cùng luồng.
 *    - Nếu dùng ở môi trường đa luồng/thực-thời-gian, cần bổ sung cơ chế
 *      đồng bộ (mutex/atomic hoặc ring buffer SPSC/MPMC chuyên dụng).
 *
 *  Mở rộng:
 *    - Nếu muốn mô phỏng CAN-FD “đủ tải” (tối đa 64 byte payload/frame),
 *      có thể đổi kích thước phần tử từ [8] → [64] và cập nhật encode/decode.
 * ============================================================================
 */

/**
 * @brief  Khởi tạo bus: đặt head=tail=0, count=0. Bộ đệm data không cần xoá vì
 *         mọi vị trí chỉ hợp lệ khi count>0 và được truy xuất qua head/tail.
 */
void bus_init(bus_t* b){
    b->head = 0;
    b->tail = 0;
    b->count = 0;
}

/**
 * @brief  Gửi (enqueue) 1 frame 8B vào bus.
 * @param  b        con trỏ tới bus
 * @param  frame8   mảng 8 byte nguồn (được sao chép vào buffer)
 * @return true     nếu xếp thành công
 *         false    nếu bus đang ĐẦY (count == 64) → KHÔNG ghi đè dữ liệu cũ
 *
 * Trình tự:
 *   1) Nếu đầy → trả false.
 *   2) Sao chép 8B vào vị trí data[head].
 *   3) head tiến 1 (quấn vòng mod 64), count++.
 */
bool bus_send(bus_t* b, const uint8_t frame8[8]){
    if (b->count >= 64) return false;               // ĐẦY → từ chối
    memcpy(b->data[b->head], frame8, 8);            // Ghi frame
    b->head = (b->head + 1) % 64;                   // Tiến con trỏ ghi (wrap)
    b->count++;                                     // Tăng số phần tử
    return true;
}

/**
 * @brief  Nhận (dequeue) 1 frame 8B từ bus.
 * @param  b            con trỏ tới bus
 * @param  frame8_out   mảng 8 byte đích để nhận dữ liệu
 * @return true         nếu lấy được 1 frame
 *         false        nếu bus đang RỖNG (count == 0)
 *
 * Trình tự:
 *   1) Nếu rỗng → trả false.
 *   2) Sao chép 8B từ vị trí data[tail] ra frame8_out.
 *   3) tail tiến 1 (quấn vòng mod 64), count--.
 */
bool bus_recv(bus_t* b, uint8_t frame8_out[8]){
    if (b->count == 0) return false;                // RỖNG → không có gì để đọc
    memcpy(frame8_out, b->data[b->tail], 8);        // Đọc frame
    b->tail = (b->tail + 1) % 64;                   // Tiến con trỏ đọc (wrap)
    b->count--;                                     // Giảm số phần tử
    return true;
}
