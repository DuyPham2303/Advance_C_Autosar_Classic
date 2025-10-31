#include "Rte_StopEngine.h"
#include "Com.h"
#include <stdio.h>

/* RTE gọi để đọc cờ StopEngine từ COM */
Std_ReturnType Rte_Read_Com_StopEngine(bool *stopPtr) {
    return Com_Get_StopEngine(stopPtr);
}

/* SWC StopEngine_Init được RTE gọi khi khởi */
void StopEngine_Init(void) {
    printf("[StopEngine_Init] StopEngine SWC initialized.\n");
}

/* Runnable chính do RTE định kỳ gọi */
void Rte_StopEngine_MainFunction(void) {
    bool stop = false;
    if (Rte_Read_Com_StopEngine(&stop) == RTE_E_OK && stop) {
        StopEngine_Run();
    } else {
        printf("[Rte_StopEngine] No StopEngine command.\n");
    }
}
