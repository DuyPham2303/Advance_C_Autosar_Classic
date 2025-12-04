#include "sensor.h"
static char* unit[TOTAL_SENSOR] =  {"độ c","rpm","vol","ampe"};
static char* sensorType[TOTAL_SENSOR] = {"Temp sensor","Speed sensor","Voltage sensor","Current sensor"};

// Hàm khởi tạo node Sensor
static Sensor* createSensor(SENSOR_ID id, float value) {
    Sensor* newSensor = (Sensor*)malloc(sizeof(Sensor));
    if (!newSensor) {
        printf("không đủ memory cấp phát\n");
        return NULL;
    }

    newSensor->sensorID = id;
    newSensor->value = value;
    newSensor->next = NULL;
    return newSensor;
}

//Hàm thêm Sensor vào đầu list 
void addSensor(Sensor** head, SENSOR_ID id, float value) {
    Sensor* newSensor = createSensor(id,value);
    newSensor->next = *head;
    *head = newSensor;
}

void printSensors(Sensor* head) {
    printf("=== Active Sensors ===\n");
    if(head == NULL){
        printf("No sensor detected\n");
        return;
    }
    while (head) {
        printf("ID: %d | Type: %s | Value: %.2f %s\n",
               head->sensorID,
               sensorType[head->sensorID],
               head->value,
               unit[head->sensorID]);
        head = head->next;
    }
}
