#ifndef RTE_SPEEDCONTROL_H
#define RTE_SPEEDCONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include "Com.h"


#define RTE_E_OK     0x00
#define RTE_E_NOT_OK 0x01

/**
 * @brief   SWC SpeedControl logic: nếu speed != NaN, in thay đổi tốc độ.
 * @param   speed   Giá trị km/h.
 */
void SpeedControl_Run(double speed);

/**
 * @brief   API RTE để SWC đọc speed từ COM.
 * @param   speedPtr   Con trỏ để RTE gán giá trị speed vào.
 * @return  RTE_E_OK nếu có giá trị mới, RTE_E_NOT_OK nếu không có.
 */
Std_ReturnType Rte_Read_Com_SetSpeed(double *speedPtr);

/**
 * @brief   Hàm khởi tạo SWC SpeedControl (do RTE gọi).
 */
void SpeedControl_Init(void);

/**
 * @brief   Runnable chính: RTE gọi định kỳ.
 */
void Rte_SpeedControl_MainFunction(void);

#endif /* RTE_SPEEDCONTROL_H */
