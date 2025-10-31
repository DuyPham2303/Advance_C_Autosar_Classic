#ifndef CANIF_H
#define CANIF_H

#include <stdint.h>
#include "PduTypes.h"

/**
 * @brief   Khởi tạo CAN Interface: gọi Can_Init của MCAL với cấu hình mặc định, rồi in log.
 */
void CanIf_Init(void);

/**
 * @brief   Callback do CAN Driver gọi khi có PDU mới (polling).
 * @param   CanRxPduId   ID PDU (trong ví dụ luôn = 0).
 * @param   PduInfoPtr   Con trỏ tới PduInfoType chứa khung CAN vừa nhận.
 */
void CanIf_RxIndication(uint8_t CanRxPduId, const PduInfoType *PduInfoPtr);

#endif /* CANIF_H */
