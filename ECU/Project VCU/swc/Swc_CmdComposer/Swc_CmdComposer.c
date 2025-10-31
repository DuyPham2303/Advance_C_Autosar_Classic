/*
 * SWC: CmdComposer
 * Giao tiếp:
 *  - SR Require: Rte_Read_SafeOut_Cmd(Safe_s* data)
 *  - C/S       : Rte_Call_CanIf_TransmitVcuCommand(thr, gear, mode, brake, alive)
 */

#include "Swc_CmdComposer.h"

/* ========================
 *   S T A T I C   S T A T E
 * ======================== */
static Swc_CmdComposer_ConfigType s_cfg = {
    /* min_period_ticks    */ 1u,     /* gửi mỗi 10ms mặc định */
    /* tx_on_change_only   */ FALSE,  /* mặc định gửi theo chu kỳ */
    /* alive_mod           */ 16u     /* 0..15 (nibble) */
};

static uint8  s_alive       = 0u;
static uint8  s_tickBudget  = 0u;     /* đếm tick để đạt min_period_ticks */
static boolean s_haveLast   = FALSE;
static Safe_s s_lastSent;             /* lần cuối đã gửi */

/* ========================
 *   L O C A L   H E L P E R S
 * ======================== */

static uint8 clamp_u8(uint8 v, uint8 vmax)
{
    return (v > vmax) ? vmax : v;
}

static boolean SafeEquals(const Safe_s* a, const Safe_s* b)
{
    if (a->ThrottleReq_pct != b->ThrottleReq_pct) return FALSE;
    if (a->Gear            != b->Gear)            return FALSE;
    if (a->Mode            != b->Mode)            return FALSE;
    if (a->Flags           != b->Flags)           return FALSE;
    if (a->BrakeActive     != b->BrakeActive)     return FALSE;
    return TRUE;
}

/* ========================
 *        P U B L I C
 * ======================== */

void Swc_CmdComposer_Init(const Swc_CmdComposer_ConfigType* cfg)
{
    if (cfg != NULL)
    {
        s_cfg = *cfg;
        /* Sanity tối thiểu */
        if (s_cfg.min_period_ticks == 0u) { s_cfg.min_period_ticks = 1u; }
        if (s_cfg.alive_mod == 0u)        { s_cfg.alive_mod = 16u; }
    }

    s_alive      = 0u;
    s_tickBudget = 0u;
    s_haveLast   = FALSE;

    /* Không gửi gì ở Init; gửi ở tick đầu theo policy period */
}

void Swc_CmdComposer_Run10ms(void)
{
    Safe_s s;
    Std_ReturnType st;

    /* 1) Đếm chu kỳ để đảm bảo tối thiểu min_period_ticks giữa 2 lần gửi */
    if (s_tickBudget < (uint8)(s_cfg.min_period_ticks - 1u))
    {
        s_tickBudget = (uint8)(s_tickBudget + 1u);
        return;
    }
    /* reset budget để có thể gửi */
    s_tickBudget = 0u;

    /* 2) Đọc SafeOut từ RTE */
    st = Rte_Read_SafeOut_Cmd(&s);
    if (st != RTE_E_OK)
    {
        /* Không đọc được: bỏ qua lần gửi này */
        return;
    }

    /* 3) Nếu chế độ gửi khi thay đổi, và đã có khung trước đó, kiểm tra khác biệt */
    if (s_cfg.tx_on_change_only == TRUE && s_haveLast == TRUE)
    {
        if (SafeEquals(&s, &s_lastSent) == TRUE)
        {
            /* Không thay đổi -> không gửi */
            return;
        }
    }

    /* 4) Chuẩn bị tham số truyền CanIf */
    const uint8  thr   = clamp_u8(s.ThrottleReq_pct, 100u);
    const uint8  gear  = (uint8)s.Gear;       /* đã chuẩn hóa trong RTE_Type.h */
    const uint8  mode  = (uint8)s.Mode;
    const boolean brk  = s.BrakeActive;
    const uint8  alive = (s_cfg.alive_mod != 0u) ? (uint8)(s_alive % s_cfg.alive_mod) : s_alive;

    /* 5) Gọi CanIf qua RTE (Client/Server) */
    st = Rte_Call_CanIf_TransmitVcuCommand(thr, gear, mode, brk, alive);
    if (st == RTE_E_OK)
    {
        /* Tăng alive counter sau khi gửi thành công */
        s_alive = (uint8)(s_alive + 1u);
        s_haveLast = TRUE;
        s_lastSent = s;
    }
    else
    {
        /* Lỗi gửi: giữ nguyên alive để retry lần sau */
    }
}
