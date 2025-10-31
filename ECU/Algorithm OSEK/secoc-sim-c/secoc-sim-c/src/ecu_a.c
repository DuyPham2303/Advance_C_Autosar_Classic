#include "common.h"
#include "ecu.h"
#include "frames.h"

/*
 * ============================================================================
 *  ECU-A (Sender) — Gửi 1 khung SecOC bảo vệ payload "speed" lên bus
 *  ----------------------------------------------------------------------------
 *  Dòng chảy (theo tinh thần AUTOSAR SecOC):
 *    1) secoc_build_frame():
 *         - Lấy FV_full mới từ Freshness TX (tăng đơn điệu).
 *         - Authentic-Data = DataID || Speed || FV_full (đúng thứ tự/endianness).
 *         - Tính CMAC/AES-128 → cắt **MSB** theo cấu hình (ví dụ 24 bit) → MAC_trunc.
 *         - Lấy **LSB** của FV_full theo cấu hình (ví dụ 8 bit) → FV_trunc.
 *         - Kết quả: frame8_t gồm [DataID(2) | Speed(2) | FV_trunc(1) | MAC_trunc(3)].
 *    2) frame8_encode(): đóng gói struct → 8 byte big-endian “trên dây”.
 *    3) bus_send(): xếp 8 byte vào hàng đợi vòng (mô phỏng CAN/CAN-FD).
 *    4) In log TX: giúp theo dõi giá trị đã gửi (DataID/Speed/FV_trunc/MAC_trunc).
 *
 *  Lưu ý:
 *    - SecOC đảm bảo **xác thực & toàn vẹn**, không che nội dung.
 *    - Nếu bus đầy (count==64) → không ghi đè, báo DROP để tránh mất dấu trình tự.
 * ============================================================================
 */
void ecu_send_speed(ecu_t* a, bus_t* bus, uint16_t speed){
    frame8_t f; 
    uint8_t raw[8];

    // 1) Xây dựng khung SecOC (tính FV_trunc + MAC_trunc bên trong)
    secoc_build_frame(&a->secoc, speed, &f);

    // 2) Mã hoá struct frame thành 8 byte theo layout cố định (big-endian)
    frame8_encode(raw, &f);

    // 3) Gửi vào "bus" vòng (enqueue). Nếu đầy → không ghi đè (trả false).
    if(!bus_send(bus, raw)){
        // Hàng đợi đã đầy: thông báo để người dùng điều chỉnh cửa sổ/jitter hoặc nhịp gửi
        printf("[%s] BUS FULL, drop\n", a->name);
    }else{
        // 4) Log gói đã gửi (hex in hoa, zero-pad để dễ đọc trong trace)
        printf("[%s] TX: data_id=0x%04X speed=%u fv_tr=0x%02X mac=%02X%02X%02X\n",
            a->name, 
            f.data_id,          // in theo big-endian khi encode ra dây
            speed,              // ví dụ: đơn vị 0.1 km/h
            f.fv_trunc,         // 8 bit thấp của FV_full
            f.mac_trunc[0],     // 3 byte cao của CMAC (MSB trước)
            f.mac_trunc[1],
            f.mac_trunc[2]);
    }
}
