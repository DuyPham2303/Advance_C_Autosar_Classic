#include <stdio.h>
#include <stdint.h>
/*
+ VẤN ĐỀ 
        - sinh vien : ten,tuoi,mssv,nganh hoc -> char,float,int
        - Lựa chọn dataype phù hợp ? 
+ ĐẶC ĐIỂM của Struct 
    - data alignment : 
        what ? 
            - yêu cầu căn chỉnh của hệ thống (cpu).
            đảm bảo CPU có thể truy cập data nhanh nhất 
        Why ? 
            - CPU truy cập bộ nhớ như thế nào ? phụ thuộc
                + natural unit -> thường là word -> là gì ?
                + word : đơn vị dữ liệu mà cpu xử lý trong 1 lần
                + memory cycle : mõi lần cpu hoặc bus truy xuất RAM, có thể lấy 
                                 1 hoặc nhiều word - phụ thuộc cache / bus width
            - Ví dụ
            struct data {
                uint8_t a;
                uint32_t b;
            }
            => theo logic thông thường chiếm 5 byte -> dẫn đến 
            CPU tốn 2 cycle để truy xuất b (chậm hơn,ko linh hoạt) 
            --------------------------------------
            address       a          b
            0x00 - 0x03   0x00       0x01 - 0x03
            0x04          0x04
            --------------------------------------
            => giải pháp ? CPU tự động căn chỉnh địa chỉ các biến sao cho
            đảm bảo tối ưu thao tác truy xuất 
        how ? 
            - CPU tự động sắp xếp các biến được cấp phát tại địa chỉ là 
            bội số của datatype (chia hết cho size datatype)
            - Nói 1 cách dễ hiểu : các member có size = n bytes sẽ được đặt 
            tại các ô nhớ có address chia hết cho 

        - double,uint64_t : 0x08 (8) ,0x10 (16),0x20 (32) ,0x30 (48)
        - uint32_t,float,int : 0x04,0x08,0x0c
        - uint16_t , short : 0x02,0x04,0x06
        - char , uint8_t (unsigned char) : 0x01,0x02,0x03
    - padding byte : 
        + byte đệm giúp căn chỉnh địa chỉ cấp phát của các member 
        + bản chất của nó giúp giảm thiểu số chu kỳ đọc của CPU khi truy xuát member
        + Vấn đề 
            - Tốn memory 
            - rủi ro truyền dữ liệu bị sai 
                -> giao tiếp MCU <-> PC Yêu cầu thống nhất data alignment để tránh lệch byte 
                    - PC (x86 - 64) : natural unit -> tối đa 8 byte
                    - stm32         : natural unit -> tối đa 4 byte
        + Giải pháp ?
            - struct packing hỗ trợ chỉ định số byte mà 1 chu kỳ lệnh 
            của CPU sẽ truy xuất 1 lần
            ví dụ :
            + cách 1:
                #pragman pack(1) -> truy cập 1 byte / lần
            + cách 2:
                struct name {
                ...
                }__attribute__((packed));
            + cách 3:
                #pragma pack(push,2)    // 1 or 2, 4, 8
                struct name {
                    ...
                }
                #pragma pack(pop)


+ Tóm tắt 
    - biến struct cũng chiếm memory, có address
    - có thể khởi tạo mảng các struct 
    - có thể truyền/trả về thông qua hàm 
*/

typedef int sensor_data;
/*
    - kích thước của member có datatype max
    - thứ tự sắp xếp của member -> ảnh hưởng phát sinh byte padding ít/nhiều/ko có
    - size = total members + padding 
*/
typedef struct data
{
    // cấp phát 8 byte : 0x00 - 0x07
    double a; 
    // câp phát 8 byte : 0x08 - 0x0f
    int b; // 0x08 - 0x0b 
    char c; //0x0c + 3 padding (0x0d - 0x0f)
}data;

//#pragma pack(1)
typedef struct A{
    uint8_t  a; // 1 byte  + 1 padding (căn chỉnh cho b)
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

/* 
    Tạo kiểu dữ liệu mới (User-defined Data type)
    - chưa được cấp pháp memory
    - các member bên trong chỉ được cấp phát address khi tạo ra các biến
    với kiểu struct datatype đã tạo
*/
typedef struct field{
    //cấp phát 2 byte : 0xc0 - 0xc1
    uint8_t a : 3;
    uint8_t b : 4;
    //cấp phát 2 byte -> 0xc2 - 0xc3 
    uint16_t c : 10;
}field;

typedef struct {
  int time;
  float temp;
  int humidity;
} Sensor_t; 
// thêm đuôi +t hoặc Type để định nghĩa 1 kiểu user-define datatype  
// theo chuẩn thiết kế - tuân theo thiết kế các kiểu datatype trong thiết kế thu viện hệ thống như stdint.h  
int main()
{

    //khai báo và truy xuất gán giá trị trực tiếp
    data dt = {.a = 12,
                  .b = 1,
                  .c = 19}; 

    data dt1 = {12,21,34}; //gán giá trị ngầm định 
    sizeof(A);
    data* pdt = &dt; 
    pdt->a = 20; // (*pdt).a = 20;
    printf("address of a = %p\n", &dt.a);
    printf("address of b = %p\n", &dt.b);
    printf("address of c = %p\n", &dt.c);

    return 0;
}