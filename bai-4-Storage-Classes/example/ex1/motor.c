#include <stdio.h> 
#include "motor.h"

void control_motor(int sensor_value){
    if(sensor_value == 0){
        printf("stop motor\n");
    }
    else{
        printf("start motor\n");
    }
}