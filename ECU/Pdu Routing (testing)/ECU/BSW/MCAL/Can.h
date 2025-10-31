#ifndef CAN_H
#define CAN_H

#include "PduTypes.h"
#include <stdint.h>

/**
 * @brief   Cấu trúc tối giản cho CAN driver (MCAL).
 *          Chỉ chứa duy nhất một controller.
 */
typedef struct {
    struct {
        uint8_t  ControllerId;   /* ID của controller (ví dụ 0) */
        uint32_t BaudRate;       /* Baudrate, ví dụ 500000 */
    } ControllerConfig[1];
} Can_ConfigType;

/**
 * @brief   Khởi tạo CAN driver (MCAL giả lập).
 * @param   ConfigPtr   Con trỏ tới cấu hình (Can_ConfigType).
 */
void Can_Init(const Can_ConfigType *ConfigPtr);

/**
 * @brief   Polling đọc frame CAN từ CSV, parse thành PduInfoType,
 *          rồi gọi CanIf_RxIndication(0, &pdu).
 */
void Can_MainFunction_Read(void);

#endif /* CAN_H */
