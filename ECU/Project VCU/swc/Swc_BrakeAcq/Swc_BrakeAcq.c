/*
 * SWC: BrakeAcq
 * Giao tiếp:
 *  - Client/Server: Rte_Call_DigIf_ReadChannel(), Rte_Call_DigIf_GetDebounced()
 *  - Sender/Receiver (Provide): Rte_Write_BrakeOut_BrakePressed()
 */

#include "Swc_BrakeAcq.h"

/* -------- Trạng thái & cấu hình nội bộ -------- */
static Swc_BrakeAcq_ConfigType s_cfg = {
    /* Giá trị mặc định an toàn; sẽ bị ghi đè nếu Init() nhận cfg != NULL */
    /* channel      */ 1u,
    /* active_high  */ TRUE,
    /* debounce_ticks */ 3u
};

static boolean s_lastDebouncedRaw = FALSE; /* mức thô sau debounce (chưa đảo cực) */
static uint8   s_debounceCnt      = 0u;
static boolean s_lastReported     = FALSE; /* giá trị đã ghi ra RTE lần trước (đã đảo cực) */

/* -------- Helpers -------- */

/* Debounce: cần N mẫu liên tiếp khác với trạng thái hiện giữ để chấp nhận đổi trạng thái */
static boolean Swc_BrakeAcq_Debounce(boolean sampleRaw)
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

/* Chuẩn hóa về “nhấn hay không” theo cực hoạt động */
static boolean Swc_BrakeAcq_ToPressed(boolean debouncedRaw)
{
    return (s_cfg.active_high == TRUE) ? debouncedRaw : (debouncedRaw ? FALSE : TRUE);
}

/* -------- Public API -------- */

void Swc_BrakeAcq_Init(const Swc_BrakeAcq_ConfigType* cfg)
{
    if (cfg != NULL)
    {
        s_cfg = *cfg;
        /* Sanity tối thiểu */
        if (s_cfg.debounce_ticks == 0u) { s_cfg.debounce_ticks = 1u; }
    }

    s_lastDebouncedRaw = FALSE;
    s_debounceCnt      = 0u;
    s_lastReported     = FALSE;

    /* Xuất trạng thái mặc định ban đầu */
    (void)Rte_Write_BrakeOut_BrakePressed(FALSE);
}

void Swc_BrakeAcq_Run10ms(void)
{
    Std_ReturnType st;
    boolean raw = FALSE;

    /* 1) Ưu tiên dùng debounce của IoHwAb nếu có; nếu trả NOT_OK, fallback đọc thô */
    st = Rte_Call_DigIf_GetDebounced(s_cfg.channel, &raw);
    if (st != RTE_E_OK)
    {
        /* đọc tức thời */
        st = Rte_Call_DigIf_ReadChannel(s_cfg.channel, &raw);
        if (st != RTE_E_OK)
        {
            /* Không đọc được phần cứng: giữ trạng thái cũ, vẫn cập nhật ra RTE để downstream thấy ổn định */
            (void)Rte_Write_BrakeOut_BrakePressed(s_lastReported);
            return;
        }
        /* dùng debounce phần mềm */
        (void)Swc_BrakeAcq_Debounce(raw);
    }
    else
    {
        /* IoHwAb đã debounce sẵn → cập nhật thô sau debounce */
        s_lastDebouncedRaw = raw;
        s_debounceCnt = 0u;
    }

    /* 2) Chuẩn hóa cực hoạt động -> pressed */
    {
        const boolean pressed = Swc_BrakeAcq_ToPressed(s_lastDebouncedRaw);

        /* 3) Ghi ra RTE (mỗi chu kỳ để phản ánh trạng thái hiện tại) */
        (void)Rte_Write_BrakeOut_BrakePressed(pressed);

        /* 4) Lưu để xử lý khi lỗi đọc */
        s_lastReported = pressed;
    }
}
