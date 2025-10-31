#include "Rte_SpeedControl.h"
#include "Com.h"
#include <stdio.h>
#include <math.h>

/* RTE gọi để đọc speed từ COM */
Std_ReturnType Rte_Read_Com_SetSpeed(double *speedPtr) {
    return Com_Get_SetSpeed(speedPtr);
}

/* SWC SpeedControl_Init được RTE gọi khi khởi */
void SpeedControl_Init(void) {
    printf("[SpeedControl_Init] SpeedControl SWC initialized.\n");
}

/* Runnable chính do RTE định kỳ gọi */
void Rte_SpeedControl_MainFunction(void) {
    double speed = NAN;
    if (Rte_Read_Com_SetSpeed(&speed) == RTE_E_OK) {
        SpeedControl_Run(speed);
    } else {
        printf("[Rte_SpeedControl] No new SetSpeed available.\n");
    }
}
