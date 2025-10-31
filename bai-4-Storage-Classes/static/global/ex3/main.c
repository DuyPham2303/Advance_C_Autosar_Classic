#include "motor.h"

int main(){
    //khởi tạo đối tượng chửa các con trỏ hàm
    MotorController motor;
    
    //khởi tạo các con trỏ hàm 
    init_motor(&motor);

    PIN pin = 2;
    int speed = 222;

    //gọi hàm thông qua con trỏ hàm
    motor.changeSpeed(pin,speed);
    motor.start(pin);
    motor.stop(pin);

  
    return 0;
}