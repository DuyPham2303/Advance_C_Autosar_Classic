#include "CanIf.h"
#include "PduR.h"
#include "Can.h"
#include <stdio.h>

/* Danh sách ID mà CanIf quan tâm (ở ví dụ chỉ 0x3F0) */
static const Can_IdType acceptList[] = {
    0x3F0  /* StopEngine hoặc SetSpeed */
};

/* Khai báo cấu hình CAN mặc định cho 1 controller */
static const Can_ConfigType Can_Config = {
    .ControllerConfig = {
        { 
          .ControllerId = 0, 
          .BaudRate     = 500000UL 
        }
    }
};

void CanIf_Init(void) {
    /* Trước hết khởi MCAL CAN driver với cấu hình Can_Config */
    Can_Init(&Can_Config);

    /* Tại đây có thể cấu hình filter hardware, danh sách PDU, v.v. */
    printf("[CanIf] Initialized (filter IDs: 0x3F0)\n");
}

void CanIf_RxIndication(uint8_t CanRxPduId, const PduInfoType *PduInfoPtr) {
    /* 1. Lọc ID */
    int found = 0;
    for (size_t i = 0; i < sizeof(acceptList)/sizeof(acceptList[0]); i++) {
        if (PduInfoPtr->id == acceptList[i]) {
            found = 1;
            break;
        }
    }
    if (!found) {
        /* Bỏ qua khung không nằm trong danh sách */
        return;
    }

    /* 2. Route sang PduR */
    PduR_RxIndication(/*srcProtocol=*/PduR_PROTOCOL_CAN,
                     /*srcPduId=*/CanRxPduId,
                     /*PduInfoPtr=*/PduInfoPtr);
}
