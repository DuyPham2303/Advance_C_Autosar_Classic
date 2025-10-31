#ifndef PDUR_H
#define PDUR_H

#include <stdint.h>
#include "PduTypes.h"

/* Enum mô tả giao thức nguồn */
typedef enum {
    PduR_PROTOCOL_CAN = 0,
    /* Nếu có LIN, FlexRay, ... */
} PduR_ProtocolType;

/* Enum mô tả module đích */
typedef enum {
    PduR_DEST_COM = 0,
    /* PduR_DEST_DCM, PduR_DEST_FIM, ... nếu mở rộng */
} PduR_DestModuleType;

/**
 * @brief   Mỗi entry trong bảng định tuyến Rx
 */
typedef struct {
    PduR_ProtocolType   srcProtocol;   /* Giao thức nguồn (CAN, LIN, ...) */
    uint16_t            srcPduId;      /* Mã do CanIf/LinIf cung cấp (CanRxPduId hoặc LinRxPduId) */
    PduR_DestModuleType destModule;    /* Module đích (COM, DCM, FiM, ...) */
    uint16_t            destPduId;     /* Mã mà module đích nhận (comRxPduId, dcmRxPduId, ...) */
} PduR_RxRoutingEntryType;

/**
 * @brief   Khởi tạo PduR (in log, khởi bảng định tuyến).
 */
void PduR_Init(void);

/**
 * @brief   Callback chung do CanIf hoặc LinIf gọi khi có RxIndication.
 * @param   srcProtocol    Giao thức nguồn (CAN hoặc LIN).
 * @param   srcPduId       Mã PDU do CanIf/LinIf cung cấp.
 * @param   PduInfoPtr     Con trỏ tới PduInfoType chứa dữ liệu.
 */
void PduR_RxIndication(PduR_ProtocolType srcProtocol,
                       uint16_t srcPduId,
                       const PduInfoType *PduInfoPtr);

#endif /* PDUR_H */
