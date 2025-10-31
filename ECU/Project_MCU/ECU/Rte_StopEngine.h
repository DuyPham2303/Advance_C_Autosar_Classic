#ifndef RTE_STOPENGINE_H
#define RTE_STOPENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include "Com.h"

#define RTE_E_OK     0x00
#define RTE_E_NOT_OK 0x01

/**
 * @brief   SWC StopEngine logic: in log tắt động cơ.
 */
void StopEngine_Run(void);

/**
 * @brief   API RTE để SWC đọc cờ StopEngine từ COM.
 * @param   stopPtr   Con trỏ để RTE gán true/false.
 * @return  RTE_E_OK nếu có cờ, RTE_E_NOT_OK nếu không có.
 */
Std_ReturnType Rte_Read_Com_StopEngine(bool *stopPtr);

/**
 * @brief   Hàm khởi tạo SWC StopEngine (do RTE gọi).
 */
void StopEngine_Init(void);

/**
 * @brief   Runnable chính: RTE gọi định kỳ.
 */
void Rte_StopEngine_MainFunction(void);

#endif /* RTE_STOPENGINE_H */
