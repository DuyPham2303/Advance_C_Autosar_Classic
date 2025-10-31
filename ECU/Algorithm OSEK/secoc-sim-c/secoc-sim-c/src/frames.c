#include "common.h"
#include "frames.h"

/*
 * ============================================================================
 *  FRAMES.C — Encode/Decode khung SecOC 8 byte
 *  ----------------------------------------------------------------------------
 *  Layout cố định (giản lược, phù hợp ví dụ SecOC trên CAN/CAN-FD):
 *      [ DataID (2B) | Speed (2B) | FV_trunc (1B) | MAC_trunc (3B) ]
 *
 *  Quy ước endianness "trên dây":
 *    - Các trường 16-bit (DataID, Speed) được mã hoá theo **big-endian**
 *      (network byte order) để nhất quán khi trao đổi giữa các máy/kiến trúc.
 *    - Trường 1B/3B (FV_trunc, MAC_trunc[3]) sao chép nguyên byte.
 *
 *  Ghi chú:
 *    - FV_trunc là LSB của FV_full (ví dụ 8 bit) — phía nhận sẽ khôi phục FV_full.
 *    - MAC_trunc là MSB của CMAC/AES-128 (ví dụ 24 bit) — dùng để xác thực nhanh.
 * ============================================================================
 */

void frame8_encode(uint8_t out[8], const frame8_t* f){
    // Mã hoá 16-bit theo big-endian ra buffer "trên dây"
    be16enc(out+0, f->data_id);   // DataID → 2B MSB trước
    be16enc(out+2, f->speed);     // Speed  → 2B MSB trước

    // Sao chép các trường 1B/3B như-is
    out[4] = f->fv_trunc;
    out[5] = f->mac_trunc[0];
    out[6] = f->mac_trunc[1];
    out[7] = f->mac_trunc[2];
}

void frame8_decode(frame8_t* f, const uint8_t raw[8]){
    // Giải mã 16-bit big-endian về giá trị host-endian
    f->data_id = be16dec(raw+0);
    f->speed   = be16dec(raw+2);

    // Sao chép lại các trường 1B/3B
    f->fv_trunc     = raw[4];
    f->mac_trunc[0] = raw[5];
    f->mac_trunc[1] = raw[6];
    f->mac_trunc[2] = raw[7];
}
