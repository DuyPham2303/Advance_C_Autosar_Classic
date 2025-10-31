/*
 * SWC: SafetyManager
 * Giao tiếp:
 *  - SR Require: Rte_Read_PedalOut_PedalPct(), Rte_Read_BrakeOut_BrakePressed(), Rte_Read_GearOut_Gear()
 *  - Mode      : Rte_Mode_DriveMode_Mode()
 *  - SR Provide: Rte_Write_SafeOut_Cmd()
 *  - Exclusive : Rte_Enter_Safety_EA(), Rte_Exit_Safety_EA()
 */

#include "Swc_SafetyManager.h"

/* ========================
 *   S T A T I C   S T A T E
 * ======================== */
static Swc_SafetyManager_ConfigType s_cfg = {
    /* brake_override_enable */ TRUE,
    /* eco_limit_pct         */ 60u,
    /* reverse_limit_pct     */ 20u,
    /* zero_in_park_neutral  */ FALSE,
    /* clamp_min_pct         */ 0u,
    /* clamp_max_pct         */ 100u,
    /* rate_limit_pct_per_tick */ 0u
};

static uint8 s_last_cmd_pct = 0u; /* để áp rate limit */

/* ========================
 *   L O C A L   H E L P E R S
 * ======================== */

static uint8 clamp_u8(uint8 v, uint8 lo, uint8 hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static uint8 apply_rate_limit(uint8 prev, uint8 now, uint8 max_step)
{
    if (max_step == 0u) return now;
    if (now > prev) {
        uint8 inc = (uint8)(now - prev);
        return (inc > max_step) ? (uint8)(prev + max_step) : now;
    } else {
        uint8 dec = (uint8)(prev - now);
        return (dec > max_step) ? (uint8)(prev - max_step) : now;
    }
}

/* ========================
 *         P U B L I C
 * ======================== */

void Swc_SafetyManager_Init(const Swc_SafetyManager_ConfigType* cfg)
{
    if (cfg != NULL)
    {
        s_cfg = *cfg;

        /* Sanity tối thiểu cho clamp */
        if (s_cfg.clamp_max_pct > 100u) s_cfg.clamp_max_pct = 100u;
        if (s_cfg.clamp_min_pct > s_cfg.clamp_max_pct) s_cfg.clamp_min_pct = s_cfg.clamp_max_pct;

        if (s_cfg.eco_limit_pct > 100u) s_cfg.eco_limit_pct = 100u;
        if (s_cfg.reverse_limit_pct > 100u) s_cfg.reverse_limit_pct = 100u;
    }

    s_last_cmd_pct = 0u;

    /* Xuất frame Safe_s mặc định để downstream có trạng thái ngay */
    Safe_s out;
    out.ThrottleReq_pct = 0u;
    out.Gear            = GEAR_P;
    out.Mode            = MODE_ECO;
    out.Flags           = 0u;
    out.BrakeActive     = FALSE;

    Rte_Enter_Safety_EA();
    (void)Rte_Write_SafeOut_Cmd(&out);
    Rte_Exit_Safety_EA();
}

void Swc_SafetyManager_Run10ms(void)
{
    uint8      pedal = 0u;
    boolean    brake = FALSE;
    Gear_e     gear  = GEAR_P;
    DriveMode_e mode = MODE_ECO;

    /* 1) Đọc input */
    (void)Rte_Read_PedalOut_PedalPct(&pedal);
    (void)Rte_Read_BrakeOut_BrakePressed(&brake);
    (void)Rte_Read_GearOut_Gear(&gear);
    mode = Rte_Mode_DriveMode_Mode();

    /* 2) Áp luật an toàn */
    uint16 flags = 0u;
    uint8  thr   = pedal;

    /* 2.1 Brake override */
    if (s_cfg.brake_override_enable == TRUE && brake == TRUE) {
        thr = 0u;
        flags |= SAFETY_FLAG_BRAKE_OVERRIDE;
    }

    /* 2.2 Zero khi P/N (tuỳ chọn) */
    if (s_cfg.zero_in_park_neutral == TRUE) {
        if (gear == GEAR_P || gear == GEAR_N) {
            thr = 0u;
            flags |= SAFETY_FLAG_ZERO_PN;
        }
    }

    /* 2.3 ECO limit */
    if (mode == MODE_ECO && thr > s_cfg.eco_limit_pct) {
        thr = s_cfg.eco_limit_pct;
        flags |= SAFETY_FLAG_ECO_LIMIT;
    }

    /* 2.4 Reverse limit */
    if (gear == GEAR_R && thr > s_cfg.reverse_limit_pct) {
        thr = s_cfg.reverse_limit_pct;
        flags |= SAFETY_FLAG_REV_LIMIT;
    }

    /* 2.5 Clamp cứng */
    if (thr < s_cfg.clamp_min_pct) { thr = s_cfg.clamp_min_pct; flags |= SAFETY_FLAG_CLAMP_MIN; }
    if (thr > s_cfg.clamp_max_pct) { thr = s_cfg.clamp_max_pct; flags |= SAFETY_FLAG_CLAMP_MAX; }

    /* 2.6 Rate limit (làm mượt lệnh xuống ECU) */
    thr = apply_rate_limit(s_last_cmd_pct, thr, s_cfg.rate_limit_pct_per_tick);

    /* 3) Gói Safe_s & ghi ra RTE trong Exclusive Area */
    Safe_s out;
    out.ThrottleReq_pct = thr;
    out.Gear            = gear;
    out.Mode            = mode;
    out.Flags           = flags;
    out.BrakeActive     = brake;

    Rte_Enter_Safety_EA();
    (void)Rte_Write_SafeOut_Cmd(&out);
    Rte_Exit_Safety_EA();

    /* 4) Lưu trạng thái */
    s_last_cmd_pct = thr;
}
