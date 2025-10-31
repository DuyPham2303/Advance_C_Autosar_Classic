#ifndef SWC_PEDALACQ_H
#define SWC_PEDALACQ_H

/*
 * SWC: PedalAcq
 * Nhiệm vụ:
 *  - Đọc điện áp bàn ga (mV) qua RTE → IoHwAb_Adc (Client–Server).
 *  - Chuẩn hoá về % (0..100), lọc EMA, chống spike, giới hạn ramp.
 *  - Ghi % ra RTE (Sender–Receiver Provide).
 */

#include "Std_Types.h"
#include "Rte_Type.h"
#include "Rte_Swc_PedalAcq.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Cấu hình runtime cho SWC PedalAcq */
typedef struct
{
    uint8  channel;            /* kênh ADC trong IoHwAb_Adc */
    uint16 v_min_mv;           /* điện áp tương ứng 0% (mV) */
    uint16 v_max_mv;           /* điện áp tương ứng 100% (mV), > v_min_mv */
    uint8  ema_alpha_q8;       /* hệ số EMA (Q8): 1..255, 0 = tắt EMA */
    uint8  ramp_pct_per_tick;  /* giới hạn thay đổi mỗi tick (0 = không giới hạn) */
    uint16 spike_mv_reject;    /* ngưỡng loại “spike” (mV); 0 = tắt */
} Swc_PedalAcq_ConfigType;

/* Khởi tạo SWC với cấu hình runtime */
void Swc_PedalAcq_Init(const Swc_PedalAcq_ConfigType* cfg);

/* Runnable 10 ms (được OS/RTE gọi theo lịch) */
void Swc_PedalAcq_Run10ms(void);

#ifdef __cplusplus
}
#endif

#endif /* SWC_PEDALACQ_H */
