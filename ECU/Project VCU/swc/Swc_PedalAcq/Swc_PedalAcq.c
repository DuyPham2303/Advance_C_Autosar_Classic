/*
 * SWC: PedalAcq
 * Giao tiếp:
 *  - Client/Server: Rte_Call_AdcIf_ReadChannel(uint8 ch, uint16* mv)
 *  - Sender/Receiver (Provide): Rte_Write_PedalOut_PedalPct(uint8 pct)
 */

#include "Swc_PedalAcq.h"

/* ========================
 *   S T A T I C   S T A T E
 * ======================== */
static Swc_PedalAcq_ConfigType s_cfg = {
    /* channel            */ 0u,
    /* v_min_mv           */ 200u,    /* mặc định loại bỏ vùng chết thấp */
    /* v_max_mv           */ 3300u,   /* phù hợp hệ 3.3V */
    /* ema_alpha_q8       */ 64u,     /* ~0.25 */
    /* ramp_pct_per_tick  */ 3u,      /* tối đa +/−3% mỗi 10ms */
    /* spike_mv_reject    */ 200u     /* bỏ mẫu nhảy >200 mV */
};

static boolean s_haveLastMv     = FALSE;
static uint16  s_last_mv        = 0u;     /* mV */
static uint8   s_pct_filt       = 0u;     /* % sau EMA */
static uint8   s_last_reported  = 0u;     /* % lần trước đã ghi ra RTE */

/* ========================
 *   L O C A L   H E L P E R S
 * ======================== */

static uint16 abs_u16(int32 x)
{
    return (uint16)((x < 0) ? -x : x);
}

static uint16 clamp_u16(uint16 v, uint16 lo, uint16 hi)
{
    return (v < lo) ? lo : (v > hi ? hi : v);
}

static uint8 clamp_u8(uint8 v, uint8 hi)
{
    return (v > hi) ? hi : v;
}

/* Tuyến tính hoá mV → %; clamp trong [v_min, v_max] */
static uint8 map_mv_to_pct(uint16 mv, uint16 vmin, uint16 vmax)
{
    uint16 cmv = clamp_u16(mv, vmin, vmax);
    uint16 span = (uint16)((vmax > vmin) ? (vmax - vmin) : 1u);
    uint32 num = (uint32)(cmv - vmin) * 100u;
    uint8 pct = (uint8)(num / span);
    if (pct > 100u) pct = 100u;
    return pct;
}

/* EMA Q8: y += (x - y)*alpha/256 */
static uint8 ema_q8(uint8 y_prev, uint8 x, uint8 alpha_q8)
{
    if (alpha_q8 == 0u) return x; /* tắt EMA */
    int16 diff = (int16)x - (int16)y_prev;
    int32 acc  = ((int32)diff * (int32)alpha_q8 + 128) >> 8; /* làm tròn */
    int16 y    = (int16)y_prev + (int16)acc;
    if (y < 0) y = 0;
    if (y > 100) y = 100;
    return (uint8)y;
}

/* Giới hạn tốc độ thay đổi mỗi tick */
static uint8 apply_ramp(uint8 prev, uint8 now, uint8 max_step)
{
    if (max_step == 0u) return now; /* không giới hạn */
    if (now > prev)
    {
        uint8 inc = (uint8)(now - prev);
        if (inc > max_step) return (uint8)(prev + max_step);
        return now;
    }
    else
    {
        uint8 dec = (uint8)(prev - now);
        if (dec > max_step) return (uint8)(prev - max_step);
        return now;
    }
}

/* ========================
 *        P U B L I C
 * ======================== */

void Swc_PedalAcq_Init(const Swc_PedalAcq_ConfigType* cfg)
{
    if (cfg != NULL)
    {
        s_cfg = *cfg;
        if (s_cfg.v_max_mv <= s_cfg.v_min_mv) { s_cfg.v_max_mv = (uint16)(s_cfg.v_min_mv + 1u); }
        /* ema_alpha_q8, ramp_pct_per_tick, spike_mv_reject: giữ nguyên theo người dùng */
    }

    s_haveLastMv    = FALSE;
    s_last_mv       = s_cfg.v_min_mv;
    s_pct_filt      = 0u;
    s_last_reported = 0u;

    /* Ghi % khởi tạo ra RTE để downstream đồng bộ tức thời */
    (void)Rte_Write_PedalOut_PedalPct(0u);
}

void Swc_PedalAcq_Run10ms(void)
{
    uint16 mv = 0u;
    Std_ReturnType rc = Rte_Call_AdcIf_ReadChannel(s_cfg.channel, &mv);
    if (rc != RTE_E_OK)
    {
        /* Không đọc được phần cứng: giữ giá trị đã công bố lần trước */
        (void)Rte_Write_PedalOut_PedalPct(s_last_reported);
        return;
    }

    /* Loại spike nếu vượt ngưỡng (và đã có mẫu trước đó) */
    if (s_cfg.spike_mv_reject != 0u && s_haveLastMv == TRUE)
    {
        uint16 delta = abs_u16((int32)mv - (int32)s_last_mv);
        if (delta > s_cfg.spike_mv_reject)
        {
            mv = s_last_mv; /* bỏ mẫu nhiễu */
        }
    }

    /* mV -> % tuyến tính + clamp */
    uint8 pct_raw = map_mv_to_pct(mv, s_cfg.v_min_mv, s_cfg.v_max_mv);

    /* EMA */
    uint8 pct_ema = ema_q8(s_pct_filt, pct_raw, s_cfg.ema_alpha_q8);

    /* Ramp limit so với giá trị đã công bố trước đó (để ra CAN mượt) */
    uint8 pct_out = apply_ramp(s_last_reported, pct_ema, s_cfg.ramp_pct_per_tick);
    pct_out = clamp_u8(pct_out, 100u);

    /* Ghi ra RTE */
    (void)Rte_Write_PedalOut_PedalPct(pct_out);

    /* Cập nhật trạng thái nội bộ */
    s_last_mv       = mv;
    s_haveLastMv    = TRUE;
    s_pct_filt      = pct_ema;
    s_last_reported = pct_out;
}
