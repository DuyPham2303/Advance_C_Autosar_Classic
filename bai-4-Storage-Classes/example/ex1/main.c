#include <stdio.h>
#include "motor.h"
#include "sensor.h"
int main(){
    read_sensor();

    printf("sensor value: %d",value);

    control_motor(value);
    
    return 0;
}
