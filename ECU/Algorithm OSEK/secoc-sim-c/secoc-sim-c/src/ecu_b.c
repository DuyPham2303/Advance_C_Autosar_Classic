#include "common.h"
#include "ecu.h"
#include "frames.h"

/*
 * ============================================================================
 *  ECU-B (Receiver) — Nhận & verify 1 khung SecOC từ bus
 *  ----------------------------------------------------------------------------
 *  Dòng chảy (rút gọn theo AUTOSAR SecOC):
 *    1) Kéo 1 frame 8B từ "bus" vòng (nếu rỗng thì return).
 *    2) Giải mã layout: [DataID(2) | Speed(2) | FV_trunc(1) | MAC_trunc(3)].
 *    3) Khôi phục Freshness đầy đủ (FV_full) từ FV_trunc:
 *         - Tìm ứng viên nhỏ nhất ≥ (last_ok+1) có LSB khớp với FV_trunc
 *           trong khoảng [last_ok+1 .. last_ok+RX_ACCEPT_WINDOW].
 *         - Nếu không tìm ra (vượt cửa sổ) → coi là “non-fresh” → DROP.
 *    4) Tính lại CMAC/AES-128 trên Authentic-Data = DataID || Speed || FV_full
 *       rồi so sánh MAC_trunc (MSB của CMAC) theo kiểu hằng-thời-gian.
 *         - Khớp → ACCEPT: cập nhật last_ok = FV_full (commit).
 *         - Lệch → DROP.
 *    5) Log kết quả (có in thêm FV_full khi ACCEPT để theo dõi đồng bộ).
 *
 *  Ghi chú:
 *    - “Acceptance window” là ngưỡng cho phép lệch đếm freshness khi nhận,
 *      nhằm chống replay nhưng vẫn chịu được mất gói/jitter. Nếu vượt cửa sổ,
 *      SecOC phải dừng verify và DROP gói.
 * ============================================================================
 */
void ecu_process_once(ecu_t* b, bus_t* bus){
    uint8_t raw[8];

    // 1) Kéo 1 frame từ bus (nếu rỗng thì không làm gì)
    if(!bus_recv(bus, raw)) return;

    // 2) Giải mã layout khung Secured I-PDU 8B
    frame8_t f; 
    frame8_decode(&f, raw);

    // 3) Verify theo SecOC: khôi phục FV_full từ FV_trunc và kiểm MAC
    uint64_t fv_full = 0;
    bool ok = secoc_verify_frame(&b->secoc, &f, &fv_full);

    // 4) Log: luôn in DataID/Speed/FV_trunc/MAC_trunc; nếu OK in thêm FV_full
    printf("[%s] RX: data_id=0x%04X speed=%u fv_tr=0x%02X mac=%02X%02X%02X -> %s",
        b->name, f.data_id, f.speed, f.fv_trunc,
        f.mac_trunc[0], f.mac_trunc[1], f.mac_trunc[2],
        ok ? "ACCEPT" : "DROP");

    if(ok){
        // Khi xác thực thành công: hiển thị FV_full để theo dõi đồng bộ hóa
        printf(" (FV_full=0x%016llX)\n", (unsigned long long)fv_full);
    }else{
        printf("\n");
    }
}
