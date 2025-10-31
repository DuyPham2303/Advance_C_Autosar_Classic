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

//Hàm cập nhật data Sensor
void updateSensor(Sensor* head, SENSOR_ID id, float newValue) {
    while(head) {
        if (head->sensorID == id) {
            head->value = newValue;
            return;
        }
        head = head->next;
    }
    printf("Sensor ID %d not found\n", id);
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
void deleteSensor(Sensor** head, SENSOR_ID id) {
    Sensor* curr = *head;
    Sensor* prev = NULL;
    while (curr != NULL) {
        if (curr->sensorID == id) {
            
            //trường hợp xóa node đầu 
            if (prev == NULL) {
                *head = curr->next; //cập nhật node đầu của list
            } 
            //trường hợp xóa ở các vị trí còn lại
            else {
                prev->next = curr->next;
            }
            free(curr);
            printf("Sensor ID %d deleted.\n", id);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Sensor ID %d not found.\n", id);
}
void freeSensors(Sensor** head) {
    Sensor* temp;
    while (*head) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
