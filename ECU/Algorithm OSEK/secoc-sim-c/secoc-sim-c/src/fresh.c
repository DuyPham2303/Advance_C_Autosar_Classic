#include "common.h"
#include "fresh.h"

/*
 * ============================================================================
 *  FRESHNESS MANAGER — TX/RX
 *  ----------------------------------------------------------------------------
 *  Mục tiêu:
 *    - TX: quản lý bộ đếm Freshness Value (FV_full, 64 bit), tăng đơn điệu
 *      mỗi lần gửi 1 PDU; cắt LSB (FV_trunc) để mang lên bus cho tiết kiệm băng thông.
 *    - RX: từ FV_trunc + trạng thái (last_ok) + "acceptance window" → khôi phục
 *      ứng viên FV_full tối thiểu ≥ last_ok+1 khớp LSB(FV_full) == FV_trunc.
 *
 *  Ghi chú an toàn:
 *    - FV_full cần được lưu/khôi phục NVM để chống replay qua lần khởi động lại.
 *    - Giá trị FV_full không được TRÀN trong vòng đời khóa; đổi khóa trước khi có nguy cơ.
 *    - "Acceptance window" cho phép chịu mất gói/jitter nhưng vẫn hạn chế replay.
 * ============================================================================
 */

/* ------------------- TX (GỬI) ------------------- */

// Khởi tạo bộ đếm FV_full của phía gửi (thường đọc từ NVM)
void fresh_tx_init(fresh_tx_t* tx, uint64_t start){ tx->counter = start; }

// Tăng và trả về FV_full mới để dùng cho frame kế tiếp.
// Lưu ý: phải tăng ĐƠN ĐIỆU; không để tràn trong vòng đời khóa.
uint64_t fresh_tx_next(fresh_tx_t* tx){
    tx->counter += 1ULL;
    return tx->counter;
}

// Cắt 'bits' bit LSB của FV_full để gửi kèm trên bus (ví dụ 8 bit).
// Trả về giá trị 8-bit (do wire field là 1 byte trong ví dụ).
// Tiền điều kiện: bits ∈ [1..8] cho layout khung 8B hiện tại.
uint8_t fresh_trunc_lsbits(uint64_t fv_full, unsigned bits){
    return (uint8_t)(fv_full & ((1u<<bits)-1u));
}

/* ------------------- RX (NHẬN) ------------------- */

// Khởi tạo phía nhận: mốc last_ok (FV_full đã verify gần nhất) & kích thước cửa sổ
void fresh_rx_init(fresh_rx_t* rx, uint64_t start, uint32_t window){
    rx->last_ok = start;
    rx->window  = window;
}

// Khôi phục ứng viên FV_full TỐI THIỂU thỏa:
//   - FV_full >= last_ok + 1
//   - (FV_full & ((1<<bits)-1)) == trunc
//   - FV_full ∈ [last_ok+1, last_ok+window]
// Tìm tuyến tính k = 0..window-1: cand = base + k; so LSB(cand) với 'trunc'.
// Nếu tìm thấy, trả true và *out_full = cand; nếu không, false (ngoài cửa sổ).
bool fresh_reconstruct_full(const fresh_rx_t* rx, uint8_t trunc, unsigned bits, uint64_t* out_full){
    uint64_t base = rx->last_ok + 1ULL;
    uint64_t mask = ((uint64_t)1 << bits) - 1ULL;
    // Duyệt trong acceptance window
    for(uint32_t k=0;k<rx->window;k++){
        uint64_t cand = base + (uint64_t)k;
        if( (cand & mask) == (uint64_t)trunc ){
            *out_full = cand;
            return true;
        }
    }
    return false; // không thể khôi phục trong cửa sổ → coi là non-fresh (DROP)
}

// Sau khi verify thành công, commit mốc last_ok = FV_full vừa chấp nhận.
// Điều này giúp lần sau RX chỉ chấp nhận FV_full tiến về phía trước.
void fresh_rx_commit(fresh_rx_t* rx, uint64_t fv_full){
    if(fv_full > rx->last_ok) rx->last_ok = fv_full;
}
