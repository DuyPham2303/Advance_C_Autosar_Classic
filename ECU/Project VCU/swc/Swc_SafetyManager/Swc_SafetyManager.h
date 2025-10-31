#ifndef SWC_SAFETYMANAGER_H
#define SWC_SAFETYMANAGER_H

/*
 * SWC: SafetyManager
 * Nhiệm vụ:
 *  - Thu thập các tín hiệu: PedalPct, BrakePressed, Gear, Mode
 *  - Áp luật an toàn: Brake override, Eco limit, Reverse limit, Clamp, Zero P/N
 *  - Giới hạn tốc độ thay đổi (rate limit) cho lệnh an toàn
 *  - Ghi Safe_s ra RTE (Sender–Receiver Provide)
 *
 * Giao tiếp RTE (khai báo trong Rte_Swc_SafetyManager.h):
 *  - Rte_Read_PedalOut_PedalPct(uint8*)
 *  - Rte_Read_BrakeOut_BrakePressed(boolean*)
 *  - Rte_Read_GearOut_Gear(Gear_e*)
 *  - Rte_Mode_DriveMode_Mode(void)
 *  - Rte_Write_SafeOut_Cmd(const Safe_s*)
 *  - (tuỳ chọn) Rte_Enter_Safety_EA() / Rte_Exit_Safety_EA()
 */

#include "Std_Types.h"
#include "Rte_Type.h"
#include "Rte_Swc_SafetyManager.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Bit flags sử dụng trong Safe_s.Flags (giữ nguyên tương thích với mẫu trước) */
#define SAFETY_FLAG_BRAKE_OVERRIDE   (1u << 0)
#define SAFETY_FLAG_ECO_LIMIT        (1u << 1)
#define SAFETY_FLAG_REV_LIMIT        (1u << 2)
#define SAFETY_FLAG_CLAMP_MIN        (1u << 3)
#define SAFETY_FLAG_CLAMP_MAX        (1u << 4)
#define SAFETY_FLAG_ZERO_PN          (1u << 5)

/* Cấu hình runtime cho SWC SafetyManager */
typedef struct
{
    /* Luật an toàn */
    boolean brake_override_enable;  /* TRUE: thắng -> ép 0% */
    uint8   eco_limit_pct;          /* MODE_ECO: max % ga (0..100) */
    uint8   reverse_limit_pct;      /* GEAR_R  : max % ga (0..100) */
    boolean zero_in_park_neutral;   /* TRUE: ở P hoặc N -> ép 0% */

    /* Giới hạn cứng */
    uint8   clamp_min_pct;          /* min % sau cùng (0..100) */
    uint8   clamp_max_pct;          /* max % sau cùng (0..100) */

    /* Giới hạn tốc độ thay đổi output mỗi tick 10ms (0 = không giới hạn) */
    uint8   rate_limit_pct_per_tick;

} Swc_SafetyManager_ConfigType;

/* Khởi tạo SWC với cấu hình runtime */
void Swc_SafetyManager_Init(const Swc_SafetyManager_ConfigType* cfg);

/* Runnable 10ms (được OS/RTE gọi theo lịch) */
void Swc_SafetyManager_Run10ms(void);

#ifdef __cplusplus
}
#endif

#endif /* SWC_SAFETYMANAGER_H */
