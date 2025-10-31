#ifndef SWC_BRAKEACQ_H
#define SWC_BRAKEACQ_H

/*
 * SWC: BrakeAcq
 * Nhiệm vụ: Đọc DIO công tắc thắng -> Debounce -> ghi BrakePressed ra RTE.
 * Phụ thuộc RTE App Header: Rte_Swc_BrakeAcq.h
 */

#include "Std_Types.h"
#include "Rte_Type.h"
#include "Rte_Swc_BrakeAcq.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Cấu hình runtime cho SWC BrakeAcq */
typedef struct
{
    uint8   channel;        /* ID kênh đọc từ IoHwAb_Digital */
    boolean active_high;    /* TRUE: mức 1 = nhấn; FALSE: mức 0 = nhấn */
    uint8   debounce_ticks; /* số mẫu liên tiếp (chu kỳ 10ms) để xác nhận đổi trạng thái */
} Swc_BrakeAcq_ConfigType;

/* Khởi tạo cấu hình & trạng thái nội bộ của SWC */
void Swc_BrakeAcq_Init(const Swc_BrakeAcq_ConfigType* cfg);

/* Runnable 10ms (được OS/RTE gọi theo lịch) */
void Swc_BrakeAcq_Run10ms(void);

#ifdef __cplusplus
}
#endif

#endif /* SWC_BRAKEACQ_H */
