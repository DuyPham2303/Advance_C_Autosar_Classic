#include <stdint.h>
/* 
    - Kết hợp bitfield để thao tác trên thanh ghi MCU
    Ưu điểm : 
        + không cần định nghĩa các macro bitmask
        + không cần thao tác bitwise phức tạp 
*/

#define BAUDRATE_2400 0
#define BAUDRATE_4800 1
#define BAUDRATE_9600 2
#define BAUDRATE_115200 3

//cấu trúc chứa các trường bit tương ứng với chức năng cần thao tác trên thanh ghi  
typedef volatile struct {
    uint8_t TX_EN       : 1; // Bit 0: Transmit Enable
    uint8_t RX_EN       : 1; // Bit 1: Receive Enable
    uint8_t BAUD_PRESC  : 2; // Bit 2-3: Baud Prescaler -> 2 bit - 2^2 = 4 : 0,1,2,3 - 00,01,10,11
    uint8_t PARITY_EN   : 1; // Bit 4: Parity Enable
    uint8_t PARITY_TYPE : 1; // Bit 5: Parity Type
    uint8_t RSVD        : 2; // Bit 6-7: Reserved
} UART_CR_BITS_t;   

struct {
    uint32_t GPIO_CR_REG;
    //....
}GPIO;

//cho phép truy cập thanh ghi dưới dạng byte hoặc bitfield
typedef union {
    uint8_t           byte_access; // Truy cập toàn bộ byte
    UART_CR_BITS_t    bits;       // Truy cập từng bit
} UART_CR_REG_t;

#define UART_CR (*((volatile UART_CR_REG_t *) 0x40001000)) // Địa chỉ thanh ghi

void init_uart(void) {
    // Cấu hình bitfield
    UART_CR.bits.TX_EN = 1;       // Bật truyền
    UART_CR.bits.RX_EN = 1;       // Bật nhận
    UART_CR.bits.BAUD_PRESC = 0;  // Hệ số baud 0
    UART_CR.bits.PARITY_EN = 0;   // Tắt parity

    // Đọc trạng thái
    if (UART_CR.bits.RX_EN) {
        // UART nhận đang bật
    }

    // Truy cập toàn bộ byte
    uint8_t cr_value = UART_CR.byte_access;
}

int main() {
    init_uart();
    return 0;
}