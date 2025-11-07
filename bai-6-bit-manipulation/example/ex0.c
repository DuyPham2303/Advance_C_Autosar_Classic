#include <stdio.h>
#include <stdint.h>

// Mô phỏng MCU gửi theo MSB trước - nhận về theo LSB
uint8_t TransmitData_MsbFirst(uint8_t data_send)
{
    uint8_t data_receive = 0;   //0b xxxx xxxx
    for (uint8_t i = 0; i < 8; i++)
    {
        // truyền MSB trước 
        /* 
             0b 10xx xxxx
             0b 0000 0001 (mask)
        >> 6    0000 0010

        */
        uint8_t bit = (data_send >> (7 - i)) & 0x01;
        /* 
                   bit = 0x00
        bit << 6 : 0x80 (MSB) - 0B 0000 0000
        OR
        receive = 0x00 - 0b 1000 0000
        */

        //nhận và xử lý MSB trước 
        data_receive |= (bit << (7 - i));
    }
    return data_receive;
}

// Mô phỏng MCU gửi theo LSB trước
uint8_t TransmitData_LsbFirst(uint8_t data_send)
{
    uint8_t data_receive = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        /* 
             0b xxxx xxx1
        >> 0 0b xxxx xxx1
             0b 0000 0001 (mask)
             0b 0000 0001 (bit)
        */
        // Lấy bit thấp nhất
        uint8_t bit = (data_send >> i) & 0x01;
        /* 
            0b 0000 0000 (receive)  
            0b 0000 0001 (bit) << 7 
            0b 1000 0000
        */

        //nhận về theo LSB trước 
        data_receive |= (bit << i);
    }
    return data_receive;
}

int main()
{
    uint8_t send = 126;
    printf("Data send: %d\n", send);

    uint8_t receive1 = TransmitData_MsbFirst(send);
    uint8_t receive2 = TransmitData_LsbFirst(send);

    printf("MSB first RX: %d\n", receive1);
    printf("LSB first RX: %d\nn", receive2);

    return 0;
}
