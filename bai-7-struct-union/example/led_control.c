#include <stdio.h>
#include <stdint.h>

#define ENABLE 1
#define DISABLE 0

typedef struct
{
    //chỉ định mỗi biến chỉ chiếm 1 bit
    uint8_t LED1 : 1;
    uint8_t LED2 : 1;
    uint8_t LED3 : 1;
    uint8_t LED4 : 1;
    uint8_t LED5 : 1;
    uint8_t LED6 : 1;
    uint8_t LED7 : 1;
    uint8_t LED8 : 1;
} LEDStatus; // total size = 1 byte
void display_LED(LEDStatus lednum)
{
    if (lednum.LED1)
        printf("LED1 is on\n");
    if (lednum.LED2)
        printf("LED2 is on\n");
    if (lednum.LED3)
        printf("LED3 is on\n");
    if (lednum.LED4)
        printf("LED4 is on\n");
    if (lednum.LED5)
        printf("LED5 is on\n");
    if (lednum.LED6)
        printf("LED6 is on\n");
    if (lednum.LED7)
        printf("LED7 is on\n");
    if (lednum.LED8)
        printf("LED8 is on\n");
}
int main()
{
    LEDStatus ledStatus = {.LED7 = ENABLE,.LED1 = ENABLE};

    // Kiểm tra trạng thái của LED 1
    display_LED(ledStatus);

    return 0;
}
