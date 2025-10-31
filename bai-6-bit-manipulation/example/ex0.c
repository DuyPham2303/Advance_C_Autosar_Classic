#include <stdio.h>
#include <stdint.h>
uint8_t ReceiveData_Msb(uint8_t data_send)
{
    uint8_t data_receive = 0;
    // mặt nạ để kiểm tra từng bit truyền đi từ MSB đến LSB
    uint8_t mask = 0x80; // 0b 1000 000

    // giả sử dữ liệu truyền đi là 8 bit
    for (uint8_t i = 0; i < 8; i++)
    {
        data_receive <<= 1; // dịch trái để nhận bit tiếp theo

        // kiểm tra nếu là bit hiện tại là 0 thì bỏ qua
        if (data_send & mask)
        {
            data_receive |= 1; // đọc về bit 1
        }
        mask >>= 1; // dịch mask để truyền bit tiếp theo
    }
    return data_receive;
}
uint8_t ReceiveData_Lsb2Msb(uint8_t data_send)
{
    uint8_t data_receive = 0;
    // mặt nạ để kiểm tra từng bit truyền đi từ LSB đến MSB
    uint8_t mask = 0x01; // 0b 1000 000

    // giả sử dữ liệu truyền đi là 8 bit
    for (uint8_t i = 0; i < 8; i++)
    {
        // kiểm tra nếu là bit hiện tại là 0 thì bỏ qua
        if (data_send & mask)
        {
            data_receive |= mask;
        }

        mask <<= 1; // dịch mask để truyền bit tiếp theo
    }
    return data_receive;
}
void SwapNumber(int* a,int* b){ 
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b; 
}
int main()
{
    uint8_t send = 250;

    uint8_t data_receive = ReceiveData_Lsb2Msb(send);
    printf("receive = %d\n",data_receive);

}