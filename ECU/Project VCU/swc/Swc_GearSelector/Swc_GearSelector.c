/*
 * SWC: GearSelector
 * Giao tiếp:
 *  - C/S: Rte_Call_DigIf_ReadChannel(ch, &level)
 *  - SR P: Rte_Write_GearOut_Gear(gear)
 */

#include "Swc_GearSelector.h"

/* ========================
 *   S T A T I C   S T A T E
 * ======================== */
static Swc_GearSelector_ConfigType s_cfg = {
    /* ch_P, ch_R, ch_N, ch_D */  10u, 11u, 12u, 13u,
    /* active_high           */  TRUE,
    /* debounce_ticks        */  3u,
    /* default_gear          */  GEAR_P,
    /* resolve_priority      */  { GEAR_P, GEAR_N, GEAR_D, GEAR_R }
};

typedef struct {
    boolean debounced;  /* giá trị sau debounce (chưa đảo cực) */
    uint8   cnt;        /* bộ đếm transitional */
} Debounce_t;

/* Debounce state cho từng kênh */
static Debounce_t s_dbP = { FALSE, 0u };
static Debounce_t s_dbR = { FALSE, 0u };
static Debounce_t s_dbN = { FALSE, 0u };
static Debounce_t s_dbD = { FALSE, 0u };

/* Số đã chọn lần trước (để giữ nếu không có kênh nào “đang chọn”) */
static Gear_e s_lastGear = GEAR_P;

/* ========================
 *   L O C A L   H E L P E R S
 * ======================== */

/* Debounce 1 kênh: cần N mẫu liên tiếp khác trạng thái hiện giữ để chấp nhận đổi */
static void Debounce_Update(Debounce_t* st, boolean sample, uint8 need_ticks)
{
    if (st == 0) { return; }
    if (sample != st->debounced) {
        uint8 c = (uint8)(st->cnt + 1u);
        if (c >= need_ticks) {
            st->debounced = sample;
            st->cnt = 0u;
        } else {
            st->cnt = c;
        }
    } else {
        st->cnt = 0u;
    }
}

/* Đọc 1 kênh + cập nhật debounce; nếu ch=UNUSED → coi như FALSE */
static void ReadAndDebounce(uint8 ch, Debounce_t* st, uint8 need_ticks)
{
    if (ch == SWC_GEARSEL_CHANNEL_UNUSED) {
        /* kênh không dùng -> luôn FALSE */
        Debounce_Update(st, FALSE, need_ticks);
        return;
    }
    boolean level = FALSE;
    Std_ReturnType rc = Rte_Call_DigIf_ReadChannel(ch, &level);
    if (rc == RTE_E_OK) {
        Debounce_Update(st, level, need_ticks);
    } else {
        /* lỗi đọc phần cứng: giữ trạng thái debounced cũ, không đổi */
    }
}

/* Ánh xạ cực: debounced raw -> “đang chọn” theo active_high/low */
static boolean ToSelected(boolean debounced_raw)
{
    return (s_cfg.active_high == TRUE) ? debounced_raw : (debounced_raw ? FALSE : TRUE);
}

/* Chọn gear theo bảng ưu tiên khi có nhiều kênh TRUE */
static Gear_e ResolveByPriority(boolean selP, boolean selR, boolean selN, boolean selD, Gear_e fallback)
{
    /* Nếu chỉ có đúng 1 kênh được chọn, trả luôn */
    uint8 count = 0u;
    if (selP) count++;
    if (selR) count++;
    if (selN) count++;
    if (selD) count++;
    if (count == 0u) {
        return fallback; /* không có chọn -> giữ số trước */
    }
    if (count == 1u) {
        if (selP) return GEAR_P;
        if (selR) return GEAR_R;
        if (selN) return GEAR_N;
        return GEAR_D;
    }

    /* Nhiều kênh: áp ưu tiên */
    for (uint8 i = 0u; i < 4u; i++) {
        Gear_e g = s_cfg.resolve_priority[i];
        if ((g == GEAR_P && selP) ||
            (g == GEAR_R && selR) ||
            (g == GEAR_N && selN) ||
            (g == GEAR_D && selD)) {
            return g;
        }
    }
    /* Không trúng ưu tiên (cấu hình sai?) → rơi về fallback */
    return fallback;
}

/* ========================
 *        P U B L I C
 * ======================== */

void Swc_GearSelector_Init(const Swc_GearSelector_ConfigType* cfg)
{
    if (cfg != 0) {
        s_cfg = *cfg;
        if (s_cfg.debounce_ticks == 0u) { s_cfg.debounce_ticks = 1u; }
    }

    s_dbP.debounced = FALSE; s_dbP.cnt = 0u;
    s_dbR.debounced = FALSE; s_dbR.cnt = 0u;
    s_dbN.debounced = FALSE; s_dbN.cnt = 0u;
    s_dbD.debounced = FALSE; s_dbD.cnt = 0u;

    s_lastGear = s_cfg.default_gear;

    /* Ghi số khởi tạo ra RTE để downstream đồng bộ tức thời */
    (void)Rte_Write_GearOut_Gear(s_lastGear);
}

void Swc_GearSelector_Run10ms(void)
{
    /* 1) Đọc & debounce 4 kênh */
    ReadAndDebounce(s_cfg.ch_P, &s_dbP, s_cfg.debounce_ticks);
    ReadAndDebounce(s_cfg.ch_R, &s_dbR, s_cfg.debounce_ticks);
    ReadAndDebounce(s_cfg.ch_N, &s_dbN, s_cfg.debounce_ticks);
    ReadAndDebounce(s_cfg.ch_D, &s_dbD, s_cfg.debounce_ticks);

    /* 2) Chuẩn hóa cực → “đang chọn” */
    const boolean selP = ToSelected(s_dbP.debounced);
    const boolean selR = ToSelected(s_dbR.debounced);
    const boolean selN = ToSelected(s_dbN.debounced);
    const boolean selD = ToSelected(s_dbD.debounced);

    /* 3) Giải quyết chọn số */
    const Gear_e resolved = ResolveByPriority(selP, selR, selN, selD, s_lastGear);

    /* 4) Ghi ra RTE & lưu lại */
    (void)Rte_Write_GearOut_Gear(resolved);
    s_lastGear = resolved;
}
