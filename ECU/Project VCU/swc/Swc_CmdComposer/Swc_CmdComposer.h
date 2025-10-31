#ifndef SWC_CMDCOMPOSER_H
#define SWC_CMDCOMPOSER_H

/*
 * SWC: CmdComposer
 * Nhiệm vụ:
 *  - Đọc Safe_s từ RTE (SR Require)
 *  - Biến đổi/thực thi chính sách TX (alive, tx-on-change, period)
 *  - Gọi CanIf_TransmitVcuCommand(...) qua RTE (Client/Server)
 */

#include "Std_Types.h"
#include "Rte_Type.h"
#include "Rte_Swc_CmdComposer.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Cấu hình runtime cho SWC CmdComposer */
typedef struct
{
    uint8   min_period_ticks;   /* số tick 10ms tối thiểu giữa 2 lần gửi (1 = 10ms) */
    boolean tx_on_change_only;  /* TRUE: chỉ gửi khi Safe_s thay đổi; FALSE: gửi định kỳ */
    uint8   alive_mod;          /* chu kỳ lặp alive counter (ví dụ 16 -> 0..15) */
} Swc_CmdComposer_ConfigType;

/* Khởi tạo SWC với cấu hình runtime */
void Swc_CmdComposer_Init(const Swc_CmdComposer_ConfigType* cfg);

/* Runnable 10ms (được OS/RTE gọi theo lịch) */
void Swc_CmdComposer_Run10ms(void);

#ifdef __cplusplus
}
#endif

#endif /* SWC_CMDCOMPOSER_H */
