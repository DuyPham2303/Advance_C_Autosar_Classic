#include <stdio.h>
// Định nghĩa các loại MCU
#define STM32 1
#define ESP32 2
#define ARDUINO 3
#define UNKNOWN_MCU 4
// Chọn loại MCU
#define MCU UNKNOWN_MCU

// Cấu hình GPIO phù hợp với MCU
#if MCU == STM32
    void gpio_init()
    {
        printf("Init GPIO for STM32F103 (AFIO, CRL/CRH registers)\n");
        // RCC_APB2ENR |= (1 << 2); // Enable GPIOA
        // GPIOA->CRL = 0x44444444; // Cấu hình GPIOA
    }

#elif MCU == ESP32
    void gpio_init()
    {
        printf("Init GPIO for ESP32 (use ESP-IDF style APIs)\n");
        // gpio_pad_select_gpio(GPIO_NUM_2);
        // gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    }

#elif MCU == ARDUINO
    // Giả sử dùng nền tảng Arduino AVR (ATmega328)
    void gpio_init()
    {
        printf("Init GPIO for Arduino UNO (DDR/PORT registers)\n");
        // DDRB |= (1 << DDB5);   // Set pin 13 (PB5) as output
        // PORTB &= ~(1 << PORTB5); // Set pin 13 to LOW
    }
#else
    #error "MCU không được định nghĩa đúng!"
#endif

int main()
{
    gpio_init();
    return 0;
}
