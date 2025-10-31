#include "common.h"
#include "aes_cmac.h"
#include "secoc.h"
#include "fresh.h"
#include "frames.h"
#include "bus.h"
#include "ecu.h"
#include <assert.h>

/*
 * ============================================================================
 *  MAIN DEMO — SecOC (CMAC/AES-128) A↔B trên "bus" vòng 8B
 *  ----------------------------------------------------------------------------
 *  Những gì sẽ in ra:
 *    - [selftest]   Kiểm tra CMAC theo RFC 4493 (1 vector chuẩn).
 *    - [TRACE]      Truy vết dự đoán phía TX: FV_full kế tiếp, FV_trunc,
 *                   Authentic-Data (12B), CMAC đầy đủ (16B), MAC_trunc (MSB N bit).
 *    - [ECU-A] TX   Khung A→B thực sự gửi lên bus (DataID, speed, FV_trunc, MAC_trunc).
 *    - [ECU-B] RX   Kết quả verify tại B (ACCEPT/DROP) + FV_full phục hồi nếu OK.
 *    - [STATE]      Trạng thái Freshness RX trước/sau (last_ok, window).
 *
 *  Chuẩn tham chiếu:
 *    - CMAC/AES-128: NIST SP 800-38B (K1/K2, padding "10*", CBC-MAC sửa đổi),
 *      RFC 4493 (đặc tả dễ đọc + test vectors). :contentReference[oaicite:1]{index=1}
 *    - SecOC: Freshness Value, Freshness Verify Value, acceptance window (DROP nếu ngoài cửa sổ). :contentReference[oaicite:2]{index=2}
 *    - AES ECB 1-block primitive trên macOS: CommonCrypto `CCCrypt` với `kCCOptionECBMode` (one-shot). :contentReference[oaicite:3]{index=3}
 * ============================================================================
 */

/* Helper: cắt MAC theo MSB N bit (giữ MSB, xóa phần LSB thừa) */
static void trace_take_msb_trunc(const uint8_t in[16], unsigned bits, uint8_t* out_bytes){
    unsigned bytes = (bits + 7u) / 8u;
    for (unsigned i = 0; i < bytes; i++) out_bytes[i] = in[i];
    unsigned rem = bits % 8u;
    if (rem) {
        uint8_t mask = (uint8_t)(0xFFu << (8u - rem));
        out_bytes[bytes - 1] &= mask;
    }
}

/* Helper: in một mảng bất kỳ theo hex với nhãn */
static void trace_hex(const char* tag, const uint8_t* p, size_t n){
    print_hex(tag, p, n);
}

/* RFC 4493 self-test (vector 1) — đảm bảo đúng thuật toán CMAC/AES-128 */
static void rfc4493_selftest(void){
    // K=2b7e151628aed2a6abf7158809cf4f3c
    // M=6bc1bee22e409f96e93d7e117393172a
    // T=070a16b46b4d4144f79bdd9dd04a287c
    uint8_t K[16] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    uint8_t M[16] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};
    uint8_t expect[16]={0x07,0x0a,0x16,0xb4,0x6b,0x4d,0x41,0x44,0xf7,0x9b,0xdd,0x9d,0xd0,0x4a,0x28,0x7c};
    uint8_t out[16]={0};
    aes_cmac_128(K, M, sizeof(M), out);
    int ok = (memcmp(expect,out,16)==0);
    printf("[selftest] RFC4493 1-block vector: %s\n", ok ? "PASS" : "FAIL");
    if(!ok){
        trace_hex("  got:   ", out, 16);
        trace_hex("  expect:", expect, 16);
    }
}

/* Truy vết dự đoán phía gửi cho A->B (không đụng logic nội bộ) */
static void trace_tx_predict_A2B(const secoc_ctx_t* ctx_a, const fresh_tx_t* ftx_a, uint16_t speed){
    // FV_full mà secoc_build_frame() sẽ dùng = counter + 1 (nó tự tăng bên trong)
    uint64_t fv_full = ftx_a->counter + 1ULL;
    uint8_t fv_tr = fresh_trunc_lsbits(fv_full, ctx_a->fv_trunc_bits);

    // Authentic-Data = DataID(2) | Speed(2) | FV_full(8)  (big-endian)
    uint8_t ad[12];
    be16enc(ad+0, ctx_a->data_id);
    be16enc(ad+2, speed);
    be64enc(ad+4, fv_full);

    // CMAC đầy đủ (16B) và MAC_trunc (MSB N bit)
    uint8_t tag[16];
    aes_cmac_128(ctx_a->key, ad, sizeof(ad), tag);
    const unsigned mac_bytes = (ctx_a->mac_trunc_bits + 7u)/8u;
    uint8_t mac_trunc[16] = {0};
    trace_take_msb_trunc(tag, ctx_a->mac_trunc_bits, mac_trunc);

    printf("[TRACE] A->B TX-pre  DataID=0x%04X Speed=%u\n", ctx_a->data_id, speed);
    printf("        next FV_full=0x%016llX  FV_trunc=0x%02X (bits=%u)\n",
           (unsigned long long)fv_full, fv_tr, ctx_a->fv_trunc_bits);
    trace_hex("        AD(12B)    = ", ad, 12);
    trace_hex("        CMAC(16B)  = ", tag, 16);
    printf("        MAC_trunc(%u b) = ", ctx_a->mac_trunc_bits);
    for(unsigned i=0;i<mac_bytes;i++) printf("%02X", mac_trunc[i]);
    printf("\n");
}

/* Tương tự: dự đoán phía gửi cho B->A (ACK) */
static void trace_tx_predict_B2A(const secoc_ctx_t* ctx_b2a_tx, const fresh_tx_t* ftx_b, uint16_t status){
    uint64_t fv_full = ftx_b->counter + 1ULL;
    uint8_t fv_tr = fresh_trunc_lsbits(fv_full, ctx_b2a_tx->fv_trunc_bits);

    uint8_t ad[12];
    be16enc(ad+0, ctx_b2a_tx->data_id);
    be16enc(ad+2, status);
    be64enc(ad+4, fv_full);

    uint8_t tag[16], mac_trunc[16]={0};
    aes_cmac_128(ctx_b2a_tx->key, ad, sizeof(ad), tag);
    const unsigned mac_bytes = (ctx_b2a_tx->mac_trunc_bits + 7u)/8u;
    trace_take_msb_trunc(tag, ctx_b2a_tx->mac_trunc_bits, mac_trunc);

    printf("[TRACE] B->A TX-pre  DataID=0x%04X Status=0x%04X\n", ctx_b2a_tx->data_id, status);
    printf("        next FV_full=0x%016llX  FV_trunc=0x%02X (bits=%u)\n",
           (unsigned long long)fv_full, fv_tr, ctx_b2a_tx->fv_trunc_bits);
    trace_hex("        AD(12B)    = ", ad, 12);
    trace_hex("        CMAC(16B)  = ", tag, 16);
    printf("        MAC_trunc(%u b) = ", ctx_b2a_tx->mac_trunc_bits);
    for(unsigned i=0;i<mac_bytes;i++) printf("%02X", mac_trunc[i]);
    printf("\n");
}

int main(void){
    /* 0) Tự kiểm thử CMAC theo RFC 4493 (bảo đảm đúng thuật toán) */
    rfc4493_selftest();  // :contentReference[oaicite:4]{index=4}

    /* 1) Thiết lập bus mô phỏng */
    bus_t bus; 
    bus_init(&bus);
    printf("[init] bus ready (ring 64x8B)\n");

    /* 2) Thiết lập ECU A (TX A->B) & ECU B (RX A->B) */
    fresh_tx_t ftx_a; fresh_tx_init(&ftx_a, FV_START_A);
    secoc_ctx_t ctx_a = {
        .key = SEC_KEY_A2B, .data_id = DATA_ID_A2B,
        .mac_trunc_bits = MAC_TRUNC_BITS, .fv_trunc_bits = FV_TRUNC_BITS,
        .ftx = &ftx_a, .frx = NULL
    };
    ecu_t ecuA = { .name="ECU-A", .secoc = ctx_a };

    fresh_rx_t frx_b; fresh_rx_init(&frx_b, FV_START_A, RX_ACCEPT_WINDOW);
    secoc_ctx_t ctx_b = {
        .key = SEC_KEY_A2B, .data_id = DATA_ID_A2B,
        .mac_trunc_bits = MAC_TRUNC_BITS, .fv_trunc_bits = FV_TRUNC_BITS,
        .ftx = NULL, .frx = &frx_b
    };
    ecu_t ecuB = { .name="ECU-B", .secoc = ctx_b };

    printf("[profile] A->B  MAC=%u bits (MSB), FV_trunc=%u bits, window=%u\n",
           MAC_TRUNC_BITS, FV_TRUNC_BITS, RX_ACCEPT_WINDOW);
    printf("[state ] B.RX last_ok=0x%016llX\n", (unsigned long long)frx_b.last_ok);

    /* 3) Demo: A gửi 5 frame speed (đơn vị 0.1 km/h): 1234, 1235, ... */
    uint16_t speed = 1234;
    for(int i=0;i<5;i++){
        uint16_t sp = (uint16_t)(speed + i);

        // 3.1) TRACE phía gửi (dự đoán theo counter hiện tại)
        trace_tx_predict_A2B(&ecuA.secoc, ecuA.secoc.ftx, sp);

        // 3.2) TX thực: build SecOC frame + encode + push bus
        ecu_send_speed(&ecuA, &bus, sp);

        // 3.3) TRACE trạng thái RX trước khi nhận
        printf("[STATE ] B.RX before: last_ok=0x%016llX window=%u\n",
               (unsigned long long)frx_b.last_ok, frx_b.window);

        // 3.4) RX thực: kéo 1 frame và verify (reconstruct FV_full trong window → CMAC check)
        ecu_process_once(&ecuB, &bus);

        // 3.5) TRACE trạng thái RX sau khi nhận
        printf("[STATE ] B.RX after : last_ok=0x%016llX\n",
               (unsigned long long)frx_b.last_ok);
        printf("------------------------------------------------------------------\n");
    }

    /* 4) (Optional) Demo B->A ACK: hoán đổi vai trò nhanh */
    fresh_tx_t ftx_b; fresh_tx_init(&ftx_b, FV_START_B);
    fresh_rx_t frx_a; fresh_rx_init(&frx_a, FV_START_B, RX_ACCEPT_WINDOW);

    secoc_ctx_t b2a_tx = { .key=SEC_KEY_B2A, .data_id=DATA_ID_B2A,
                           .mac_trunc_bits=MAC_TRUNC_BITS, .fv_trunc_bits=FV_TRUNC_BITS,
                           .ftx=&ftx_b, .frx=NULL };
    secoc_ctx_t b2a_rx = { .key=SEC_KEY_B2A, .data_id=DATA_ID_B2A,
                           .mac_trunc_bits=MAC_TRUNC_BITS, .fv_trunc_bits=FV_TRUNC_BITS,
                           .ftx=NULL, .frx=&frx_a };
    ecu_t ECU_B_sender   = { .name="ECU-B", .secoc=b2a_tx };
    ecu_t ECU_A_receiver = { .name="ECU-A", .secoc=b2a_rx };

    printf("[profile] B->A  MAC=%u bits (MSB), FV_trunc=%u bits, window=%u\n",
           MAC_TRUNC_BITS, FV_TRUNC_BITS, RX_ACCEPT_WINDOW);
    printf("[state ] A.RX last_ok=0x%016llX\n", (unsigned long long)frx_a.last_ok);

    // 4.1) TRACE TX B->A (dùng trường speed như "status code")
    trace_tx_predict_B2A(&ECU_B_sender.secoc, ECU_B_sender.secoc.ftx, 0x00AA);

    // 4.2) TX/RX thực
    ecu_send_speed(&ECU_B_sender, &bus, 0x00AA);
    ecu_process_once(&ECU_A_receiver, &bus);
    printf("[STATE ] A.RX after : last_ok=0x%016llX\n",
           (unsigned long long)frx_a.last_ok);

    return 0;
}
