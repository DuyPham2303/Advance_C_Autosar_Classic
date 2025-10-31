#include "PduR.h"
#include "Com.h"
#include <stdio.h>

/* Kích thước bảng: chỉ 1 entry trong ví dụ này */
#define PDUR_RX_TABLE_SIZE  1

/* Bảng định tuyến Rx */
static const PduR_RxRoutingEntryType PduR_RxRoutingTable[PDUR_RX_TABLE_SIZE] = {
    /* srcProtocol,    srcPduId, destModule,    destPduId */
    { PduR_PROTOCOL_CAN, 0,        PduR_DEST_COM, 0 }

};

void PduR_Init(void) {
    printf("[PduR] Init (1 entry)\n");
}

void PduR_RxIndication(PduR_ProtocolType srcProtocol,
                       uint16_t srcPduId,
                       const PduInfoType *PduInfoPtr)
{
    for (uint16_t i = 0; i < PDUR_RX_TABLE_SIZE; i++) {
        const PduR_RxRoutingEntryType *entry = &PduR_RxRoutingTable[i];
        if (entry->srcProtocol == srcProtocol && entry->srcPduId == srcPduId) {
            if (entry->destModule == PduR_DEST_COM) {
                Com_RxIndication((uint8_t)entry->destPduId, PduInfoPtr);
            }
            return;
        }
    }
    printf("[PduR] No routing for protocol=%u, pduId=%u\n",
           (unsigned)srcProtocol, (unsigned)srcPduId);
}
