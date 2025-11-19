#include <stdio.h>
#include <stdint.h>

// Mô phỏng MCU gửi theo MSB trước - nhận về theo LSB
uint8_t TransmitData_MsbFirst(uint8_t data_send)
{
    uint8_t data_receive = 0;   //0b xxxx xxxx
    for (uint8_t i = 0; i < 8; i++)
    {
        uint8_t bit = (data_send >> (7 - i)) & 0x01;

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
        // Lấy bit thấp nhất
        uint8_t bit = (data_send >> i) & 0x01;

        //nhận về theo LSB trước 
        data_receive |= (bit << i);
    }
    return data_receive;
}

int main()
{
    uint8_t send = 254;
    printf("Data send: %d\n", send);

    uint8_t receive1 = TransmitData_MsbFirst(send);
    uint8_t receive2 = TransmitData_LsbFirst(send);

    printf("MSB first RX: %d\n", receive1);
    printf("LSB first RX: %d\nn", receive2);

    return 0;
}
