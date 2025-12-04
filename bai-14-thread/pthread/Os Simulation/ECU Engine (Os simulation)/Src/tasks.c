#include "Os.h"
#include "Os_event.h"
#include <stdio.h>
#include <stdlib.h>

void task_sensor() {

    printf("[task_sensor] : reading sensor...\n");
    /*đặt Event cho task_enginecontrol*/
    //...
}

void task_enginecontrol() {
    EventMaskType ev = 0u;  //biến cục bộ đọc về event mong muốn 
    uint8_t errorflag = 0;  //cờ cục bộ mô phỏng tín hiệu lỗi
    srand(time(0));         //khởi tạo seed sinh số ngẫu nhiên

    for(;;){
        /*đợi event từ task mong muốn*/
        //...
        
        /*Đọc ra event đã đợi vào biến ev*/
        //...

        if (/* kiểm tra event đã đọc về từ biến ev */) {

            /*Giả lập phát hiện lỗi ngẫu nhiên -> sử dụng hàm rand()*/
            //...

            if(/* giả sử errorflag là giá trị ngẫu nhiên nào đó*/){
                printf("[task Control] Error detected! Signaling Diagnostic\n");
                
                /*đặt event cho task mong muốn */
                //...
            }
            else{
                printf("[task Control] Control command ready. Signaling Actuator\n");
                /*đặt event cho task mong muốn */
                //...
            }
            /* reset event cho task hiện tại */
            //...
        }
        //printf("[Task_enginecontrol] is running\n");
    }
}

void task_actuator() {
    EventMaskType ev = 0u;
    
    for(;;){
        /*đợi event từ task mong muốn*/
        //...
        
        /*Đọc ra event đã đợi vào biến ev*/
        //...

        if (/* kiểm tra event đã đọc về từ biến ev */){
           
            printf("[task Actuator] Executing control signal...\n");
            /*đặt event cho task mong muốn */
            //...
            
            /* reset event cho task hiện tại */
            //...
        }
    }
}

void task_diagnostic() {
    EventMaskType ev = 0u;
   
    for(;;){
        /*đợi event từ task mong muốn*/
        //...
        
        /*Đọc ra event đã đợi vào biến ev*/
        //...

        if (/* kiểm tra event đã đọc về từ biến ev */){
            
            printf("[Diagnostic] Handling error...\n");
            /*đặt event cho task mong muốn */
            //...
            
            /* reset event cho task hiện tại */
            //...
        }
        //printf("[Task_diagnostic] is running\n");
    }
}

void task_CAN(){
    
}