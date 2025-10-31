#include "Can.h"
#include "CanIf.h"
#include "csv_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_DATA_BYTES 8

/* Giả lập trạng thái controller: 0 = chưa init, 1 = đã init */
static int canInitialized = 0;
/* Lưu giữ con trỏ cấu hình hiện tại để có thể truy cập nếu cần */
static const Can_ConfigType *Can_ConfigPtr = NULL;

void Can_Init(const Can_ConfigType *ConfigPtr) {
    if (ConfigPtr == NULL) {
        printf("[Can] ERROR: Null ConfigPtr passed to Can_Init\n");
        return;
    }
    Can_ConfigPtr = ConfigPtr;
    canInitialized = 1;
    printf("[Can] Initialized controller %u at baudrate %lu\n",
           (unsigned)ConfigPtr->ControllerConfig[0].ControllerId,
           (unsigned long)ConfigPtr->ControllerConfig[0].BaudRate);
}

void Can_MainFunction_Read(void) {
    if (!canInitialized) {
        return;
    }

    /* Gọi csv_lib để lấy chuỗi PDU (ví dụ "0x3F0 8 03 13 BD ...") */
    char *frameStr = getDataPdu("can");
    if (!frameStr) {
        /* Không có frame mới, hoặc lỗi mở file */
        return;
    }

    PduInfoType rxPdu;
    memset(&rxPdu, 0, sizeof(rxPdu));

    /* 1) Lấy CAN ID */
    char *token = strtok(frameStr, " ");
    if (!token) {
        free(frameStr);
        return;
    }
    errno = 0;
    unsigned long idParsed = strtoul(token, NULL, 0);
    if (errno != 0) {
        free(frameStr);
        return;
    }
    rxPdu.id = (Can_IdType)idParsed;

    /* 2) Lấy DLC */
    token = strtok(NULL, " ");
    if (!token) {
        free(frameStr);
        return;
    }
    errno = 0;
    unsigned long dlcParsed = strtoul(token, NULL, 10);
    if (errno != 0 || dlcParsed > MAX_DATA_BYTES) {
        free(frameStr);
        return;
    }
    rxPdu.length = (PduLengthType)dlcParsed;

    /* 3) Lấy dữ liệu byte */
    for (uint8_t i = 0; i < rxPdu.length; i++) {
        token = strtok(NULL, " ");
        if (!token) {
            rxPdu.sdu[i] = 0;
            continue;
        }
        errno = 0;
        unsigned long byteVal = strtoul(token, NULL, 16);
        if (errno != 0 || byteVal > 0xFF) {
            free(frameStr);
            return;
        }
        rxPdu.sdu[i] = (uint8_t)byteVal;
    }

    free(frameStr);

    /* Gọi lên CanIf */
    CanIf_RxIndication(/*CanRxPduId=*/0, &rxPdu);
}
