#include <stdio.h>
#include <stdint.h>
/*
Khi muốn bật/tắt 1 chân GPIO, bạn phải:

Ghi một bit tại vị trí cụ thể

Đọc/mask/mã hóa các bit

→ Việc thao tác từng bit bằng shift và mask dễ gây lỗi, khó đọc và khó maintain code.
*/

typedef union
{
    uint16_t pinAll; // Truy cập toàn bộ chân 
    struct
    {
        uint16_t Pin_0 : 1;
        uint16_t Pin_1 : 1;
        uint16_t Pin_2 : 1;
        uint16_t Pin_3 : 1;
        uint16_t Pin_4 : 1;
        uint16_t Pin_5 : 1;
        uint16_t Pin_6 : 1;
        uint16_t Pin_7 : 1;
        uint16_t Pin_8 : 1;
        uint16_t Pin_9 : 1;
        uint16_t Pin_10 : 1;
        uint16_t Pin_11 : 1;
        uint16_t Pin_12 : 1;
        uint16_t Pin_13 : 1;
        uint16_t Pin_14 : 1;
        uint16_t Pin_15 : 1;
    } bits;
}GPIO_Pin_select;
