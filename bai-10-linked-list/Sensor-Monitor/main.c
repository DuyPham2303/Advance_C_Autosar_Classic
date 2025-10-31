#include "sensor.h"
int main() {
    Sensor* sensorList = NULL;

    addSensor(&sensorList, TEMP_SENSOR, 36.5);
    addSensor(&sensorList, SPEED_SENSOR, 101.2);
    addSensor(&sensorList, VOLTAGE_SENSOR, 75.0);

    printSensors(sensorList);

    updateSensor(sensorList,VOLTAGE_SENSOR,95);
    printf("\nSau cập nhật:\n");
    printSensors(sensorList);

    deleteSensor(&sensorList, SPEED_SENSOR);
    printf("\nSau xoá:\n");
    printSensors(sensorList);

    freeSensors(&sensorList);

    printSensors(sensorList);
    return 0;
}
