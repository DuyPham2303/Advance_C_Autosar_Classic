#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>

jmp_buf jump_buffer;
typedef enum{
    NO_ERROR,
    ERROR
}SensorStatus;

#define MAX_VALUE 50

int read_sensor_data()
{
    int sensor_value = rand() % 100;

    if (sensor_value > MAX_VALUE)
    {
        printf("Sensor error! Value too high: %d\n", sensor_value);
        longjmp(jump_buffer, ERROR); // nhảy về điểm an toàn
    }

    return sensor_value;
}

void Task_ReadSensor(int *out_data)
{
    printf("Reading sensor...\n");
    *out_data = read_sensor_data();
    printf("Sensor OK: %d\n", *out_data);
}

void Task_Process(int data)
{
    printf("Processing data: %d\n", data);
    // Giả lập xử lý...
}

void Task_Control(int data)
{
    printf("Control actuator based on: %d\n", data);
    // Giả lập điều khiển...
}


int main()
{
   
    while(1)
    {
        int data;

        if (setjmp(jump_buffer) == ERROR)
        {
   
            printf("\nLoop Start\n");
        }
        else
        {
            
            printf("Jumped back safely after sensor error!\n");
        }
        Task_ReadSensor(&data);
        Task_Process(data);
        Task_Control(data);
        sleep(1);
    }

    printf("\nSystem terminated.\n");
    return 0;
}
