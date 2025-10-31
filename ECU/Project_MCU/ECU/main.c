#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      /* sleep() */
#include <pthread.h>     /* pthreads */
#include "Com.h"
#include "Can.h"         /* Để có prototype Can_MainFunction_Read() */
#include "Rte_SpeedControl.h"
#include "Rte_StopEngine.h"



/**
 * @brief   Hàm polling riêng biệt chạy trong thread,
 *          gọi Can_MainFunction_Read() mỗi 1 giây.
 */
void *CanTask(void *arg) {
    (void)arg;
    while (1) {
        Can_MainFunction_Read();
        sleep(1);
    }
    return NULL;
}

/**
 * @brief   Hàm khởi tạo toàn bộ hệ thống:
 *          - Com_Init() khởi CanIf → Can_Init + PduR_Init.
 *          - RTE Init gọi SWC_Init.
 */
static void System_Init(void) {
    Com_Init();  /* gồm CanIf_Init (⇒ Can_Init) và PduR_Init */
    printf("[RTE] Init Start\n");
    SpeedControl_Init();
    StopEngine_Init();
    printf("[RTE] Init Done\n");
}

int main(void) {
    pthread_t can_thread;

    /* 1. Khởi toàn bộ BSW / RTE / SWC */
    System_Init();

    /* 2. Tạo thread cho CAN polling */
    if (pthread_create(&can_thread, NULL, CanTask, NULL) != 0) {
        perror("Failed to create CAN thread");
        exit(1);
    }

    while (1) {
        
        /* Gọi RTE runnables mỗi giây */

        Rte_SpeedControl_MainFunction();

       
        sleep(1);

    }

    /* Khi đã test xong, ta có thể dừng chương trình (ở đây exit) */
    printf("\n[Test Done] Exiting...\n");
    exit(0);
    return 0;
}
