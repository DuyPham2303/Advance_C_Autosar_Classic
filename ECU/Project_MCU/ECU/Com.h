#ifndef COM_H
#define COM_H

#include <stdint.h>
#include <stdbool.h>
#include "PduTypes.h"

typedef uint8_t Std_ReturnType;
#define RTE_E_OK     0x00
#define RTE_E_NOT_OK 0x01

/**
 * @brief   Khởi Com: trong đó sẽ khởi các layer CanIf và PduR.
 */
void Com_Init(void);

/**
 * @brief   COM nhận PDU từ PduR và decode thành hai lệnh:
 *          - 0x03 = SetSpeed
 *          - 0x04 = StopEngine
 *
 * @param   RxPduId      Mã PDU (ở ví dụ này luôn = 0).
 * @param   PduInfoPtr   PDU do PduR gửi xuống.
 */
void Com_RxIndication(uint8_t RxPduId, const PduInfoType *PduInfoPtr);

/**
 * @brief   SWC/ RTE gọi để lấy giá trị tốc độ (km/h) cuối cùng.
 * @param   speedPtr    Con trỏ để COM gán giá trị vào.
 * @return  RTE_E_OK nếu có giá trị, RTE_E_NOT_OK nếu không có.
 */
Std_ReturnType Com_Get_SetSpeed(double *speedPtr);

/**
 * @brief   SWC/ RTE gọi để lấy cờ StopEngine. Sau khi đọc, COM reset nội bộ về false.
 * @param   stopPtr     Con trỏ để COM gán true/false.
 * @return  RTE_E_OK nếu có cờ StopEngine, RTE_E_NOT_OK nếu không có.
 */
Std_ReturnType Com_Get_StopEngine(bool *stopPtr);

#endif /* COM_H */
