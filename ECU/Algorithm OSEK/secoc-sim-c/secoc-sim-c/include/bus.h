#ifndef BUS_H
#define BUS_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * ============================================================================
 *  "BUS" GIẢ LẬP MẠNG CAN/CAN-FD DẠNG VÒNG (RING BUFFER) CHO KHUNG 8 BYTE
 *  ----------------------------------------------------------------------------
 *  Mục đích:
 *    - Hàng đợi vòng (circular/ring buffer) tối giản để mô phỏng “bus”
 *      truyền tải frame giữa các ECU trong bộ mô phỏng SecOC.
 *    - Dung lượng mặc định: 64 khung, mỗi khung 8 byte.
 *
 *  Kiến trúc & tính chất:
 *    - Ba chỉ số: head (ghi), tail (đọc), count (0..64).
 *    - head/tail quấn về 0 khi tới cuối mảng (wrap-around).
 *    - Gửi/nhận O(1), không cấp phát động.
 *    - Không ghi đè khi đầy: nếu đầy, send() trả false.
 *
 *  Hành vi khi đầy/rỗng:
 *    - ĐẦY (count == 64): không xếp thêm (send() → false).
 *    - RỖNG (count == 0): không có khung để đọc (recv() → false).
 *
 *  Đa luồng:
 *    - Không khoá; phù hợp mô phỏng tuần tự hoặc SPSC.
 *    - Nếu đa luồng/thời gian thực, cần đồng bộ riêng.
 *
 *  CAN / CAN-FD & kích thước frame:
 *    - Ở đây cố định 8 byte để gọn minh hoạ.
 *    - Nếu mô phỏng CAN-FD “đúng cỡ” (payload tới 64 byte), đổi chiều thứ hai
 *      của mảng từ [8] → [64] và cập nhật encode/decode tương ứng.
 *
 *  Mẫu sử dụng:
 *      bus_t bus; bus_init(&bus);
 *      uint8_t f[8] = { 0xDE,0xAD,0xBE,0xEF,0x00,0x01,0x02,0x03 }; // ví dụ payload
 *      if (!bus_send(&bus, f)) {
 *          // bus đầy
 *      }
 *      uint8_t out[8];
 *      if (bus_recv(&bus, out)) {
 *          // xử lý out
 *      }
 *
 *  Bảo trì:
 *    - Không phụ thuộc nền tảng; không cấp phát.
 *    - Có thể thay dung lượng 64 bằng cách chỉnh kích thước mảng và ràng buộc count.
 * ============================================================================
 */

// Bộ đệm vòng trong bộ nhớ cho "CAN/CAN-FD" với khung 8 byte (giản lược)
typedef struct {
    uint8_t data[64][8];  // 64 frame, mỗi frame 8 byte (đổi 8 -> 64 nếu mô phỏng CAN-FD full payload)
    size_t  head;         // vị trí ghi tiếp theo (0..63), quấn về 0 khi tới cuối
    size_t  tail;         // vị trí đọc tiếp theo (0..63), quấn về 0 khi tới cuối
    size_t  count;        // số frame đang có trong hàng đợi (0..64)
} bus_t;

/**
 * @brief  Khởi tạo bus (xóa trạng thái, đặt head=tail=0, count=0).
 */
void bus_init(bus_t* b);

/**
 * @brief  Gửi (enqueue) 1 frame 8 byte vào bus (nếu chưa đầy).
 * @return true nếu xếp thành công; false nếu bus đầy (không ghi đè).
 */
bool bus_send(bus_t* b, const uint8_t frame8[8]);

/**
 * @brief  Nhận (dequeue) 1 frame 8 byte từ bus (nếu không rỗng).
 * @return true nếu lấy được 1 frame; false nếu bus rỗng.
 */
bool bus_recv(bus_t* b, uint8_t frame8_out[8]);

#endif /* BUS_H */
