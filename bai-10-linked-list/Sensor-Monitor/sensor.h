#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum SENSOR_ID{
    TEMP_SENSOR,
    SPEED_SENSOR,
    VOLTAGE_SENSOR,
    CURRENT_SENSOR,
    TOTAL_SENSOR
}SENSOR_ID;

typedef struct Sensor {
    SENSOR_ID sensorID;
    float value;
    struct Sensor* next;
}Sensor;

void printSensors(Sensor* head);
void addSensor(Sensor** head, SENSOR_ID id, float value);
void updateSensor(Sensor* head, SENSOR_ID id, float newValue);
void deleteSensor(Sensor** head, SENSOR_ID);
void freeSensors(Sensor** head);