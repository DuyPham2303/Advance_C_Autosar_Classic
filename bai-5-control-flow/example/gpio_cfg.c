#include <assert.h>
#include <stdint.h>

#define CHECK_GPIO_Pin(condition,error) assert(condition && #error)

#define GPIO_BASE_ADDR  ((volatile uint32_t *)0x2000000UL) //định nghĩa địa chỉ base của GPIO 

void GPIO_Config(uint8_t pinNumber) {
    //chỉ cho phép số chân GPIO từ 0 đến 15
    CHECK_GPIO_Pin(pinNumber < 16,số chân tối đa cho phép phải nhỏ hơn 16);  // dừng chương trình nếu truyền giá trị lớn hơn 15

    // Cấu hình thanh ghi giả định
    volatile uint32_t *gpio_reg = (GPIO_BASE_ADDR + pinNumber * 4);
}

int main(void) {
    //GPIO_Config(5); 
    GPIO_Config(25);  
    return 0;
}
