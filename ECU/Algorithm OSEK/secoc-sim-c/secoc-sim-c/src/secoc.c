#include "common.h"
#include "secoc.h"
#include "aes_cmac.h"

/*
 * ============================================================================
 *  take_msb_trunc()
 *  ----------------------------------------------------------------------------
 *  Cắt ngắn MAC theo **MSB (bit có trọng số cao)** với độ dài 'bits'.
 *  - CMAC/AES-128 trả về thẻ đầy đủ 16 byte (128 bit). Trong SecOC, để tiết kiệm
 *    băng thông, ta thường chỉ mang **N bit đầu** (ví dụ 24/32/64 bit) lên bus.
 *  - Quy tắc cắt: lấy các byte từ đầu mảng `in` (MSB trước). Nếu 'bits' không
 *    chia hết cho 8, ta **mask** byte cuối để giữ đúng số bit MSB và xoá phần LSB.
 *
 *  Lưu ý an toàn (chuẩn CMAC):
 *  - Độ dài MAC sử dụng (Tlen) nên **cố định** cho suốt vòng đời một khóa để
 *    tránh suy yếu tính bảo mật. Việc chọn Tlen là **trade-off** an ninh/băng thông.
 *    (Tham chiếu CMAC: NIST SP 800-38B; AES-CMAC: RFC 4493)
 */
static void take_msb_trunc(const uint8_t in[16], unsigned bits, uint8_t* out_bytes){
    unsigned bytes = (bits+7)/8;
    for(unsigned i=0;i<bytes;i++) out_bytes[i]=in[i];
    // Nếu 'bits' không bội số của 8, xoá phần bit thấp ở byte cuối — chỉ giữ MSB
    unsigned rem = bits % 8;
    if(rem){
        uint8_t mask = (uint8_t)(0xFFu << (8-rem));
        out_bytes[bytes-1] &= mask;
    }
}

/*
 * ============================================================================
 *  secoc_build_frame()
 *  ----------------------------------------------------------------------------
 *  Xây dựng 1 Secured I-PDU 8B từ payload `speed` theo tinh thần AUTOSAR SecOC:
 *    1) Lấy FV_full từ Freshness TX (tăng đơn điệu) → fv_trunc = LSB(FV_full).
 *    2) Authentic-Data = DataID(2) || Speed(2) || FV_full(8)  (endianness BE).
 *    3) Tính CMAC/AES-128(key, Authentic-Data) → lấy **MSB mac_trunc_bits**.
 *    4) Gói frame: [DataID2 | Speed2 | FV_trunc1 | MAC_truncNbytes].
 *
 *  Ghi chú:
 *  - SecOC bảo vệ **xác thực + toàn vẹn**; không mã hoá payload.
 *  - DataID được đưa vào dữ liệu xác thực để ràng buộc MAC với đúng PDU.
 *  - CMAC/AES-128: xem NIST SP 800-38B & RFC 4493.
 */
void secoc_build_frame(secoc_ctx_t* ctx, uint16_t speed, frame8_t* out){
    // 1) FV_full từ Freshness TX (được quản lý bởi ctx->ftx)
    uint64_t fv_full = fresh_tx_next(ctx->ftx);
    uint8_t  fv_tr  = fresh_trunc_lsbits(fv_full, ctx->fv_trunc_bits);

    // 2) Authentic-Data = DataID(2) || Speed(2) || FV_full(8)  (trên dây: big-endian)
    uint8_t ad[12];
    be16enc(ad+0, ctx->data_id);
    be16enc(ad+2, speed);
    be64enc(ad+4, fv_full);

    // 3) Tính CMAC/AES-128 (thẻ đầy đủ 16B)
    uint8_t tag[16];
    (void)aes_cmac_128(ctx->key, ad, sizeof(ad), tag);

    // 4) Đóng gói khung 8B theo layout mô phỏng
    out->data_id = ctx->data_id;
    out->speed   = speed;
    out->fv_trunc= fv_tr;
    take_msb_trunc(tag, ctx->mac_trunc_bits, out->mac_trunc);
}

/*
 * ============================================================================
 *  secoc_verify_frame()
 *  ----------------------------------------------------------------------------
 *  Xác thực 1 Secured I-PDU “đến”:
 *    1) Kiểm DataID đúng kênh (chống cross-PDU).
 *    2) Khôi phục FV_full từ FV_trunc trong **acceptance window** (>= last_ok+1).
 *       - Thất bại (vượt cửa sổ) ⇒ coi là non-fresh ⇒ DROP.
 *    3) Dựng lại Authentic-Data và tính CMAC/AES-128.
 *    4) So sánh MAC_trunc (MSB) theo cách **constant-time-ish** (không early-return).
 *       - Khớp ⇒ commit last_ok = FV_full ⇒ ACCEPT (trả true).
 *       - Lệch ⇒ DROP (trả false).
 *
 *  Ghi chú:
 *  - Freshness/acceptance window là cơ chế chống replay trong SecOC.
 *  - AES-CMAC primitive triển khai theo chuẩn; trên macOS dùng CommonCrypto:
 *    `CCCrypt(..., kCCOptionECBMode, ...)` để mã hoá 1 khối 128 bit trong CMAC.
 */
bool secoc_verify_frame(secoc_ctx_t* ctx, const frame8_t* in, uint64_t* out_fv_full){
    // 1) Ràng PDU: DataID phải khớp với kênh cấu hình
    if(in->data_id != ctx->data_id) return false;

    // 2) Khôi phục FV_full từ FV_trunc trong cửa sổ (>= last_ok+1, khớp LSB)
    uint64_t fv_full = 0;
    if(!fresh_reconstruct_full(ctx->frx, in->fv_trunc, ctx->fv_trunc_bits, &fv_full)){
        return false; // ngoài cửa sổ → non-fresh → DROP
    }

    // 3) Dựng lại Authentic-Data theo cùng quy tắc ở bên gửi
    uint8_t ad[12];
    be16enc(ad+0, ctx->data_id);
    be16enc(ad+2, in->speed);
    be64enc(ad+4, fv_full);

    // 4) Tính lại CMAC và cắt MSB để so sánh
    uint8_t tag[16], trunc[3]; // 3 byte cho ví dụ MAC_TRUNC_BITS=24
    (void)aes_cmac_128(ctx->key, ad, sizeof(ad), tag);
    take_msb_trunc(tag, ctx->mac_trunc_bits, trunc);

    // So sánh MAC cắt ngắn theo kiểu "constant-time-ish": không thoát sớm
    unsigned ok = 1u;
    for(unsigned i=0;i<3;i++){ ok &= (unsigned)(trunc[i] == in->mac_trunc[i]); }

    if(ok){
        // Cập nhật mốc freshness sau khi xác thực thành công
        fresh_rx_commit(ctx->frx, fv_full);
        if(out_fv_full) *out_fv_full = fv_full;
        return true;
    }
    return false;
}
