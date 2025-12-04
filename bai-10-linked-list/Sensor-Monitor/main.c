#include "sensor.h" 
#include <stdlib.h>
#include <unistd.h>
int main() {
    srand(0);

    int data = 0;
    while(1){
        data = rand() % 10; //0 - 9
        printf("data = %d\n",data);
        sleep(1);
    }

    // Sensor* Monitor = NULL;
    // addSensor(&Monitor,TEMP_SENSOR,23.4);
    // addSensor(&Monitor,SPEED_SENSOR,50);

    // printSensors(Monitor);
    //Mô phỏng quản lý thông tin cảm biến
    /* 
        Menu 
        1) Hiển thị các cảm biến đã thêm vào list
        2) Thêm cảm biến mới vào list 
        3) xóa cảm biến khỏi list
        4) Thoát -> xóa toàn bộ cảm biến
        5) Chạy mô phỏng cảm biến 
            - tạo biến running == 1 -> chạy trong while loop
            - gọi hàm updateSensor
            - Gọi hàm rand() để sinh số ngẫu nhiên 
            - tạo delay mô phỏng thay đổi dữ liệu theo tgian 
            - gán running == 0 
                -> break khỏi while loop 
                -> dừng mô phỏng
                -> return Menu
    */
    return 0;
}
