#include "SpeedControl.h"
#include <stdio.h>
#include <math.h>

void SpeedControl_Run(double speed) {
    if (!isnan(speed)) {
        printf("[SpeedControl] Setting vehicle speed to %.2f km/h\n", speed);
    } else {
        printf("[SpeedControl] Invalid speed (NaN)\n");
    }
}
