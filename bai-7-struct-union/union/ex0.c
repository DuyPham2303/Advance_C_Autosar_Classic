#include <stdio.h>
#include <stdint.h>

/*
    - members chia sẻ chung 1 vùng nhớ khi mà nó được cấp phát địa chỉ
    - size union = member (max) + padding byte
        + TH1: max datatype
        + TH2: số lượng byte max
*/

typedef union data
{
    // cấp phát 4 byte : 0x00 - 0x03
    uint8_t a;  // a(0x00) ,  3 padding (0x01 - 0x03)
    uint32_t b; // b(0x00 - 0x03)
    uint16_t c; // c(0x00 - 0x02)
} data;

typedef union Data 
{
    //cấp phát 16 byte : 0x00 - 0x0f
    uint8_t  a;	    //a(0x00) , 15 padding 
    //b[0](0x00 - 0x01)
    //b[1](0x02 - 0x03)
    //b[2](0x04 - 0x05)
    //b[3](0x06 - 0x07)
    //b[4](0x08 - 0x09)
    uint16_t b[5];	// 6 padding
    uint32_t c;	// c(0x00 - 0x03)  , 12 padding
    double   d;	// d(0x00 - 0x07) , 8 padding
}Data;
/* 
    MCU A -> MCU B

    data = 23
    - baudrate : 9600,115200,4800
    - start bit
    - stop bit : 1/2 bits
    - parity bit : even/odd bit

    => Frame 
*/
int main()
{
    sizeof(Data);

    data dt;
    /* 
        0x03        0x02        0x01        0x00
        00000000    00000001    00000010    01011000
    */
    dt.a = 23;      //00010111
    dt.b = 70000;   //00000001 00010001 01110000
    dt.c = 600;     //00000010 01011000

    printf("a = %p\n",&dt.a);
    printf("b = %p\n",&dt.b);
    printf("c = %p\n",&dt.c);

    return 0;
}