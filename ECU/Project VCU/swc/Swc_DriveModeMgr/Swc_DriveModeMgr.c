/*
 * SWC: DriveModeMgr
 * Giao tiếp:
 *  - Client/Server: Rte_Call_DigIf_ReadChannel(channel, &level)
 *  - Mode Port    : Rte_Switch_DriveMode_Mode(mode), Rte_Mode_DriveMode_Mode()
 */

#include "Swc_DriveModeMgr.h"

/* ========================
 *   S T A T I C   S T A T E
 * ======================== */
static Swc_DriveModeMgr_ConfigType s_cfg = {
    /* channel               */ 2u,
    /* debounce_ticks        */ 3u,
    /* toggle_on_press       */ TRUE,
    /* pressed_is_high       */ TRUE,
    /* normal_level_is_high  */ FALSE,
    /* initial_mode          */ MODE_ECO
};

static boolean     s_lastDebouncedRaw = FALSE;
static uint8       s_debounceCnt      = 0u;
static boolean     s_lastPressed      = FALSE;   /* chỉ dùng khi toggle_on_press = TRUE */
static DriveMode_e s_modeCurrent      = MODE_ECO;

/* ========================
 *      L O C A L S
 * ======================== */

/* Debounce: cần N mẫu liên tiếp khác trạng thái hiện tại để chấp nhận thay đổi */
static boolean Dmm_Debounce(boolean sampleRaw)
{
    if (sampleRaw != s_lastDebouncedRaw)
    {
        s_debounceCnt = (uint8)(s_debounceCnt + 1u);
        if (s_debounceCnt >= s_cfg.debounce_ticks)
        {
            s_lastDebouncedRaw = sampleRaw;
            s_debounceCnt = 0u;
        }
    }
    else
    {
        s_debounceCnt = 0u;
    }
    return s_lastDebouncedRaw;
}

/* Yêu cầu chuyển mode qua RTE nếu khác với hiện tại */
static void Dmm_RequestSwitch(DriveMode_e desired)
{
    if (desired != s_modeCurrent)
    {
        const Std_ReturnType st = Rte_Switch_DriveMode_Mode(desired);
        if (st == RTE_E_OK)
        {
            s_modeCurrent = desired;
            /* (tuỳ chọn) có thể gọi Rte_SwitchAck_DriveMode_Mode() ở nơi phù hợp */
        }
        else
        {
            /* Không đổi s_modeCurrent nếu switch không thành công */
        }
    }
}

/* ========================
 *       P U B L I C
 * ======================== */

void Swc_DriveModeMgr_Init(const Swc_DriveModeMgr_ConfigType* cfg)
{
    if (cfg != NULL)
    {
        s_cfg = *cfg;
        if (s_cfg.debounce_ticks == 0u) { s_cfg.debounce_ticks = 1u; }
        if (s_cfg.initial_mode != MODE_ECO && s_cfg.initial_mode != MODE_NORMAL)
        {
            s_cfg.initial_mode = MODE_ECO;
        }
    }

    s_lastDebouncedRaw = FALSE;
    s_debounceCnt      = 0u;
    s_lastPressed      = FALSE;

    /* Thiết lập mode khởi tạo:
       - lấy từ config (ưu tiên)
       - đồng bộ với RTE bằng Switch (để đảm bảo các consumer thấy mode đúng) */
    s_modeCurrent = s_cfg.initial_mode;
    (void)Rte_Switch_DriveMode_Mode(s_modeCurrent);
}

void Swc_DriveModeMgr_Run10ms(void)
{
    Std_ReturnType st;
    boolean raw = FALSE;

    /* 1) Đọc trạng thái mức kênh số */
    st = Rte_Call_DigIf_ReadChannel(s_cfg.channel, &raw);
    if (st != RTE_E_OK)
    {
        /* lỗi phần cứng: bỏ qua tick này */
        return;
    }

    /* 2) Debounce phần mềm */
    (void)Dmm_Debounce(raw);

    /* 3) Chính sách điều khiển mode */
    if (s_cfg.toggle_on_press == TRUE)
    {
        /* Phát hiện “press” với cực pressed_is_high */
        const boolean pressed = (s_cfg.pressed_is_high == TRUE) ? s_lastDebouncedRaw
                                                                : (s_lastDebouncedRaw ? FALSE : TRUE);
        /* Rising edge: false -> true */
        if ((pressed == TRUE) && (s_lastPressed == FALSE))
        {
            /* Toggle mode */
            const DriveMode_e next = (s_modeCurrent == MODE_ECO) ? MODE_NORMAL : MODE_ECO;
            Dmm_RequestSwitch(next);
        }
        s_lastPressed = pressed;
    }
    else
    {
        /* Level-mapped: mức tín hiệu quyết định mode */
        const boolean levelIsNormal = (s_cfg.normal_level_is_high == TRUE) ? s_lastDebouncedRaw
                                                                           : (s_lastDebouncedRaw ? FALSE : TRUE);
        const DriveMode_e desired = levelIsNormal ? MODE_NORMAL : MODE_ECO;
        Dmm_RequestSwitch(desired);
    }
}
