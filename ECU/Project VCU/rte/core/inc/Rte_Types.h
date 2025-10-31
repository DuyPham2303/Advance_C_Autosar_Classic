#ifndef RTE_TYPE_H
#define RTE_TYPE_H

#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===========================================
 * Kiểu dùng chung toàn hệ thống (Global RTE Types)
 * =========================================== */

/* Bàn số DRNP */
typedef enum {
    GEAR_P = 0,
    GEAR_R = 1,
    GEAR_N = 2,
    GEAR_D = 3
} Gear_e;

/* Chế độ lái */
typedef enum {
    MODE_ECO = 0,
    MODE_NORMAL = 1
} DriveMode_e;

/* Gói lệnh an toàn đưa xuống ECU động cơ */
typedef struct {
    uint8       ThrottleReq_pct; /* 0..100 % */
    Gear_e      Gear;
    DriveMode_e Mode;
    uint16      Flags;           /* bit flags: BRAKE_OVERRIDE, ECO_LIMIT, REV_LIMIT ... */
    boolean     BrakeActive;     /* true nếu thắng đang nhấn */
} Safe_s;

/* (tùy chọn) Kiểu ADC/DIO chuẩn hóa */
typedef uint16 Adc_mV_t;     /* mV đọc từ ADC sau khi scale */
typedef uint8  Percent_t;    /* 0..100% */

/* (tùy chọn) Result code riêng của cổng dịch vụ IoHwAb/CanIf */
#ifndef RTE_CS_OK
# define RTE_CS_OK   ((Std_ReturnType)E_OK)
#endif
#ifndef RTE_CS_NOT_OK
# define RTE_CS_NOT_OK ((Std_ReturnType)E_NOT_OK)
#endif

#ifdef __cplusplus
}
#endif

#endif /* RTE_TYPE_H */
