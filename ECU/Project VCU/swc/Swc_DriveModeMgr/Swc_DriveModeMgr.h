#ifndef SWC_DRIVEMODEMGR_H
#define SWC_DRIVEMODEMGR_H

/*
 * SWC: DriveModeMgr
 * Nhiệm vụ:
 *  - Đọc kênh DIO của nút/chế độ lái (Client–Server qua RTE)
 *  - Quản lý DriveMode.Mode (Mode port) bằng Rte_Switch_DriveMode_Mode()
 *  - Hỗ trợ 2 chính sách: toggle theo lần bấm hoặc ánh xạ mức tín hiệu → mode
 */

#include "Std_Types.h"
#include "Rte_Type.h"
#include "Rte_Swc_DriveModeMgr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Cấu hình runtime cho SWC DriveModeMgr */
typedef struct
{
    uint8       channel;              /* ID kênh số đọc từ IoHwAb_Digital */
    uint8       debounce_ticks;       /* số mẫu liên tiếp (chu kỳ 10ms) để xác nhận đổi trạng thái */
    boolean     toggle_on_press;      /* TRUE: bấm 1 lần → toggle mode; FALSE: dùng level-mapped */
    boolean     pressed_is_high;      /* khi toggle_on_press=TRUE: mức 1 là “đang bấm” nếu TRUE */
    boolean     normal_level_is_high; /* khi toggle_on_press=FALSE: mức 1 = NORMAL nếu TRUE, ngược lại ECO */
    DriveMode_e initial_mode;         /* mode khởi tạo mong muốn (ECO/NORMAL) */
} Swc_DriveModeMgr_ConfigType;

/* Khởi tạo SWC với cấu hình runtime */
void Swc_DriveModeMgr_Init(const Swc_DriveModeMgr_ConfigType* cfg);

/* Runnable 10ms (được OS/RTE gọi theo lịch) */
void Swc_DriveModeMgr_Run10ms(void);

#ifdef __cplusplus
}
#endif

#endif /* SWC_DRIVEMODEMGR_H */
