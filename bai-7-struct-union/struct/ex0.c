#include <stdio.h>
#include <stdint.h>
/*
    sinh vien : ten,tuoi,mssv,nganh hoc -> char,float,int
    + data alignment : yêu cầu căn chỉnh của hệ thống (cpu)
     -> quy định là các member trong
    struct sẽ được cấp phát địa chỉ
    có giá trị là bội số của kiểu dữ liệu mà nó khai báo
        - double,uint64_t : 0x08 (8) ,0x10 (16),0x20 (32) ,0x30 (48)
        - uint32_t,float,int : 0x04,0x08,0x0c
        - uint16_t , short : 0x02,0x04,0x06
        - char , uint8_t (unsigned char) : 0x01,0x02,0x03
    + padding byte : byte đệm được thêm vào để đảm bảo data alignment
    + size
*/

typedef int sensor_data;
/*
    - kích thước của member có datatype max
    - thứ tự sắp xếp của member
    - size of struct = total members + padding byte
*/
typedef struct data
{
    // cấp phát 8 byte : 0x00 - 0x07
    double a; 
    // câp phát 8 byte : 0x08 - 0x0f
    int b; // 0x08 - 0x0b 
    char c; //0x0c + 3 padding (0x0d - 0x0f)
}data;

typedef struct A{
    uint8_t  a; // 1 byte  
    uint16_t b; // 2 byte
    uint32_t c; // 4 byte
}A;
data dt;
data *pdt;
sensor_data a;

/* 
    - bit-field
    + chỉ áp dụng cho member khai báo trên struct
    + các member sẽ không có địa chỉ cụ thể
    + các member có cùng datatype chia sẻ chung 1 vùng nhớ 
*/
typedef struct field{
    //cấp phát 2 byte : 0xc0 - 0xc1
    uint8_t a : 3;
    uint8_t b : 4;
    //cấp phát 2 byte -> 0xc2 - 0xc3 
    uint16_t c : 10;
}field;
int main()
{

    uint8_t data; //2^8 - 1 = 255 -> 0 - 63 (2^6 - 1)
    dt.a = 21;
    A a;
    sizeof(field);
    pdt = &dt;
    pdt->a = 20;
    printf("address of a = %p\n", &a.a);
    printf("address of b = %p\n", &a.b);
    printf("address of c = %p\n", &a.c);

    return 0;
}