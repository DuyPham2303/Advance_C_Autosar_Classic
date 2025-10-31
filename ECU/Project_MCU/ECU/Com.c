#include "Com.h"
#include "CanIf.h"
#include "PduR.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>

/* Biến tĩnh lưu trạng thái do COM xử lý */
static double s_currentSpeed   = NAN;
static bool   s_stopRequested  = false;
static bool   s_speedReceived  = false;

void Com_Init(void) {
    /* Khởi CanIf và PduR ngay từ Com_Init */
    CanIf_Init();
    PduR_Init();
    printf("[Com] Initialized (calling CanIf_Init & PduR_Init)\n");
}

void Com_RxIndication(uint8_t RxPduId, const PduInfoType *PduInfoPtr) {
    (void)RxPduId;

    printf("[Com] RxIndication: ID=0x%03X, DLC=%u, Data=",
           (unsigned)PduInfoPtr->id,
           (unsigned)PduInfoPtr->length);
    for (uint8_t i = 0; i < PduInfoPtr->length; i++) {
        printf("%02X ", PduInfoPtr->sdu[i]);
    }
    printf("\n");

    if (PduInfoPtr->length < 3) {
        /* Cần ít nhất byte lệnh + 2 byte raw */
        return;
    }

    uint8_t cmd = PduInfoPtr->sdu[0];
    if (cmd == 0x03) {
        /* Lệnh SetSpeed: hai byte raw theo little-endian */
        uint16_t raw = ((uint16_t)PduInfoPtr->sdu[2] << 8)
                       | (uint16_t)PduInfoPtr->sdu[1];
        double speed = raw / 100.0;
        s_currentSpeed  = speed;
        s_speedReceived = true;
        printf("[Com] Decoded SetSpeed: raw=0x%04X (%u) → speed=%.2f km/h\n",
               raw, raw, speed);
    }
    else if (cmd == 0x04) {
        /* Lệnh StopEngine */
        s_stopRequested = true;
        printf("[Com] Decoded StopEngine: stopRequested = true\n");
    }
    else {
        printf("[Com] Unknown command: 0x%02X\n", cmd);
    }
}

Std_ReturnType Com_Get_SetSpeed(double *speedPtr) {
    if (s_speedReceived) {
        *speedPtr = s_currentSpeed;
        s_speedReceived = false;  /* Reset sau khi đọc */
        return RTE_E_OK;
    } else {
        return RTE_E_NOT_OK;
    }
}

Std_ReturnType Com_Get_StopEngine(bool *stopPtr) {
    if (s_stopRequested) {
        *stopPtr = true;
        s_stopRequested = false;  /* Reset sau khi đọc */
        return RTE_E_OK;
    } else {
        *stopPtr = false;
        return RTE_E_NOT_OK;
    }
}
