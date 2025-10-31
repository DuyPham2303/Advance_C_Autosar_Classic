#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
typedef enum{
    TASK_ID_SENSOR,
    TASK_ID_CONTROL,
    TASK_ID_ACTUATOR,
    TASK_ID_DIAGOSTIC,
    ISR_CAN_ID,
    TASK_ID_MAX
}TASK_ID;
// Condition variables and mutex
pthread_mutex_t list_mutex[TASK_ID_MAX] = {PTHREAD_MUTEX_INITIALIZER};

pthread_cond_t list_conditional[TASK_ID_MAX] = {PTHREAD_COND_INITIALIZER};

#define evSensorDataReady (1 << 0)
#define evControlCmdReady (1 << 1)
#define evErrorDetected   (1 << 2)

uint8_t EventReg;

bool setEvent(uint8_t bitmask){
    return EventReg |= bitmask;
}
bool ClearEvent(uint8_t bitmask){
    return EventReg &= ~bitmask;
}
void WaitEvent(uint8_t bitmask){
    while(!(EventReg & bitmask));
}
// TASK_Sensor
void* task_sensor(void* arg) {
    while (1) {
        //usleep(10000); // giả lập alarm mỗi 10ms
        sleep(1);
        printf("reading sensor...\n");
        //khóa mutex để đọc cảm biến và kích hoạt event
        pthread_mutex_lock(&list_mutex[TASK_ID_SENSOR]);
        printf("[Sensor] Data ready\n");
        
        while(!setEvent(evSensorDataReady));
        pthread_cond_signal(&list_conditional[TASK_ID_CONTROL]);    //thông báo cho task_control sau khi đọc xong dữ liệu

        //mở khóa mutex để cho luồng khác thực thi
        pthread_mutex_unlock(&list_mutex[TASK_ID_SENSOR]);
    }
    return NULL;
}

// TASK_Control
void* task_control(void* arg) {
    while (1) {

        pthread_mutex_lock(&list_mutex[TASK_ID_CONTROL]);

        //chờ đến khi event được kích hoạt mới tiếp tục xử lý
        WaitEvent(evSensorDataReady);
        pthread_cond_wait(&list_conditional[TASK_ID_CONTROL],&list_mutex[TASK_ID_CONTROL]);
        printf("[Control] Processing sensor data...\n");

        // Giả lập phát hiện lỗi ngẫu nhiên
        bool has_error = rand() % 5 == 0;


        if(has_error) {
            while(!setEvent(evErrorDetected));                     //kích hoạt event cho task_Diagnostic
            pthread_cond_signal(&list_conditional[TASK_ID_DIAGOSTIC]);  //thông báo cho luồng xử lý task_Diagnostic
            printf("[Control] Error detected! Signaling Diagnostic\n");
        } else {
            while(!setEvent(evControlCmdReady));                  //kích hoạt event cho task_Actuator
            pthread_cond_signal(&list_conditional[TASK_ID_ACTUATOR]);   //thông báo cho luồng xử lý task_Actuator
            printf("[Control] Control command ready. Signaling Actuator\n");
            while(ClearEvent(evSensorDataReady));
        }

        pthread_mutex_unlock(&list_mutex[TASK_ID_CONTROL]);
    }
    return NULL;
}

// TASK_Actuator
void* task_actuator(void* arg) {
    while (1) {
        pthread_mutex_lock(&list_mutex[TASK_ID_ACTUATOR]);

        //chờ cho đến khi event được kích hoạt 
        WaitEvent(evControlCmdReady);
        pthread_cond_wait(&list_conditional[TASK_ID_ACTUATOR],&list_mutex[TASK_ID_ACTUATOR]);
    
        while(ClearEvent(evControlCmdReady));
        printf("[Actuator] Executing control command...\n");

        pthread_mutex_unlock(&list_mutex[TASK_ID_ACTUATOR]);
    }
    return NULL;
}

// TASK_Diagnostic
void* task_diagnostic(void* arg) {
    while (1) {
        pthread_mutex_lock(&list_mutex[TASK_ID_DIAGOSTIC]);

        WaitEvent(evErrorDetected);
        pthread_cond_wait(&list_conditional[TASK_ID_ACTUATOR],&list_mutex[TASK_ID_ACTUATOR]);
        while(ClearEvent(evErrorDetected));
        printf("[Diagnostic] Handling error...\n");
        
        pthread_mutex_unlock(&list_mutex[TASK_ID_DIAGOSTIC]);
    }
    return NULL;
}

// ISR_CAN_RX (mô phỏng)
void* isr_can_rx(void* arg) {
    while (1) {
        sleep(3); // Mỗi 3s có 1 CAN message khẩn cấp

        pthread_mutex_lock(&list_mutex[ISR_CAN_ID]);
        // ISR kích hoạt trực tiếp TASK_Control hoặc TASK_Diagnostic
        bool emergency_error = rand() % 2;

        if (emergency_error) {
            while(!setEvent(evErrorDetected));
            pthread_cond_signal(&list_conditional[TASK_ID_DIAGOSTIC]);
            printf("[ISR] Emergency error! Triggering Diagnostic\n");
        } else {
            while(!setEvent(evControlCmdReady));
            pthread_cond_signal(&list_conditional[TASK_ID_CONTROL]);
            printf("[ISR] Emergency sensor event! Triggering Control\n");
        }

        pthread_mutex_unlock(&list_mutex[ISR_CAN_ID]);
    }
    return NULL;
}

// Main
int main() {
    pthread_t tid_sensor, tid_control, tid_actuator, tid_diag, tid_isr;

    pthread_create(&tid_sensor, NULL, task_sensor, NULL);
    pthread_create(&tid_control, NULL, task_control, NULL);
    pthread_create(&tid_actuator, NULL, task_actuator, NULL);
    pthread_create(&tid_diag, NULL, task_diagnostic, NULL);
    pthread_create(&tid_isr, NULL, isr_can_rx, NULL);

    pthread_join(tid_sensor, NULL);
    pthread_join(tid_control, NULL);
    pthread_join(tid_actuator, NULL);
    pthread_join(tid_diag, NULL);
    pthread_join(tid_isr, NULL);

    return 0;
}
