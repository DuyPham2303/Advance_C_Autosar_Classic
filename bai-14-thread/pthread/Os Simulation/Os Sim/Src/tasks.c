#include "Os.h"
#include "Os_event.h"
#include <stdio.h>
#include <stdlib.h>
atomic_int counter = 0;

void task_sensor() {
    printf("[task_sensor] : reading sensor...%d\n",atomic_load(&counter));
    atomic_fetch_add(&counter,1);
    //Kích hoạt luồng xử lý tín hiệu cảm biến
    (void)SetEvent(TASK_ID_EngineControl,EV_DataReady);
}

void task_enginecontrol() {
    EventMaskType ev = 0u;
    uint8_t errorflag = 0;
    srand(time(0));

    //for(;;){
    while(atomic_load(&counter) < 20){
        (void)WaitEvent(TASK_ID_EngineControl,EV_DataReady);

        /* Đọc mặt nạ sự kiện hiện tại*/
        (void)GetEvent(TASK_ID_EngineControl, &ev);

        if (ev & EV_DataReady) {

            // Giả lập phát hiện lỗi ngẫu nhiên
            errorflag = rand() % 5;

            if(errorflag == 3){
                printf("[task Control] Error detected! Signaling Diagnostic\n");
                (void)SetEvent(TASK_ID_Diagnostic,EV_TriggerError);
            }
            else{
                printf("[task Control] Control command ready. Signaling Actuator\n");
                (void)SetEvent(TASK_ID_Actuator,EV_ControlCmdReady);
                (void)ClearEvent(TASK_ID_EngineControl,EV_DataReady);
            }
        }
        //printf("[Task_enginecontrol] is running\n");
    }
}

void task_actuator() {
    EventMaskType ev = 0u;
    
    //for(;;){
    while(atomic_load(&counter) < 20){
        (void)WaitEvent(TASK_ID_Actuator,EV_ControlCmdReady);

        (void)GetEvent(TASK_ID_Actuator,&ev);

        if(ev & EV_ControlCmdReady){
            //output signal to actuator 
            printf("[task Actuator] Executing control signal...\n");
            //clear event từ task_control
            (void)ClearEvent(TASK_ID_Actuator,EV_ControlCmdReady);
        }
        //printf("[Task_actuator] is running\n");
    }
}

void task_diagnostic() {
    EventMaskType ev = 0u;
   
    //for(;;){
    while(atomic_load(&counter) < 20){
        (void)WaitEvent(TASK_ID_Diagnostic,EV_TriggerError);

        (void)GetEvent(TASK_ID_Diagnostic,&ev);

        if(ev & EV_TriggerError){
            //output signal to actuator 
            printf("[Diagnostic] Handling error...\n");
            //clear event từ task_control
            (void)ClearEvent(TASK_ID_Diagnostic,EV_TriggerError);
        }
        //printf("[Task_diagnostic] is running\n");
    }
}
