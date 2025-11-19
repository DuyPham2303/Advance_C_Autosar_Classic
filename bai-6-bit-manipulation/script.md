#include <stdint.h>
quản lý thông tin việc sử dụng phương tiên di chuyển user 

int    4 byte (2^32 - 1)
float 
double 

int8_t , uint8_t (1 byte - 8 bit ) -> 2^8 - 1 = 0b 0000 0000

phương tiện
uint8_t xe hơi  (1 or 0)
uint8_t xe máy  (1 or 0)
uint8_t máy bay (1 or 0)
uint8_t tàu hỏa (1 or 0)
uint8_t trạng thái (đi 1 mình / gia đình)
total = 1 byte (tối ưu ? -> 5 bit) 

thao tác bit -> ứng dụng :
- lập trình trên thanh ghi
- Giao tiếp giữa các MCU (truyền thông SPI,I2C,UART) -
    -> SPI_SendData(uint8_t data) , uint8_t SPI_ReceiveData() 

kiến thức cần thiết : truth table , logic gate 
bitwise operator : ~ , | , & , ^
logic gate 
- AND (a & b) : phép nhân - nếu input có giá trị 0 -> output = 0
- OR  (a | b) : phép cộng - nếu input có giá trị 1 -> output = 1
- NOT (b = ~a): phép đảo - input = 0 -> output = 0 (ngược lại)
- XOR         : 
        + nếu input giống nhau -> output = 0 
        + nếu intput khác nhau -> output = 1
- NAND
- NOR 
---------------------------------
Shift bit 

left shift : 0b 0000 0001 << n
- các bit bên phải sẽ được dịch sang trái n bit 
    + n bit bên trái được dịch ra khỏi phạm vi
    + n bit 0 được thêm vào bên phải
=> 0b 0000 0001 << 1 : 2^0 = 1
   0b 0000 0010 << 1 : 2^1 = 2
   0b 0000 0100 << 1 : 2^2 = 4
   0b 0000 1000 << 1 : 2^3 = 8
   0b 0001 0000 << 1 : 2^4 = 16
 
right shift : 0b 1000 0001 >> n
- các bit bên trái sẽ được dịch sang phải n bit 
    + n bit bên phải được dịch ra khỏi phạm vi
    + n bit 0 được thêm vào bên trái
0b 1000 0001 >> 3
0b 0001 0000   

//ví dụ quản lý việc lựa chọn cách phối đồ của 1 người 
//-> khai báo các macro thao tác trên từng bit tượng trưng cho nhiều lựa chọn
#define GENDER 1 << 0 // Bit 0: Giới tính (0 = Nữ, 1 = Nam) // 0b 0000 0001 
#define TSHIRT 1 << 1 // Bit 1: Áo thun (0 = Không, 1 = Có) // 0b 0000 0010
#define HAT    1 << 2 // Bit 2: Nón (0 = Không, 1 = Có)  // 0b 0000 0100
#define SHOES  1 << 3 // Bit 3: Giày (0 = Không, 1 = Có)   // 0b 0000 1000

bitmask - định nghĩa các macro đại diện cho 
mặt nạ bit dùng để thao tác với bit cần làm việc (đặt/xóa/đọc) giá trị

- 1u : đảm bảo luôn làm việc với giá trị số dương (unsigned int)
- tránh lỗi dịch bit dấu
- dich lớn hơn 31 bit -> chạm đến bit dấu 
- đối với kiểu 64-bit -> dùng ull
---------------------------------

0b xxxx xxxx (ko xác định)
OR 
0b 0000 0100 mask

(1 -> 1 or 0 -> 1)
(0/1 or 0 = chính nó)
void setbit(uint8_t *option , uint8_t mask){
        *option |= mask; 
}

0b xxxx xxxx (ko xác định)

0b 0000 1000 mask 
0b 1111 0111 ~mask
AND
0b xxxx xxxx

(1 -> 0, 0 -> 0)
(1/0 AND 1 -> chính nó)

void resetbit(uint8_t *option , uint8_t mask){
        *option &= ~mask; 
}

0b xxxx xxxx -> 0b 0000 1000
AND
0b 0000 1000 


(0/1 & 0 = 0)

(0/1 & 1 = chính nó)

bool checkbit(uint8_t option, uint8_t mask){
    return (option & mask); 
}

set nhiều bit ? gọi ra các macro kèm bitwise OR 

0b 0000 0001
0b 0000 0010 OR
0b 0000 0100 OR
0b 0000 0111
-------------------------------
Quản lý các tính năng xe hơi 
- các lựa chọn cho 
    + màu sắc           (2 bit : 2^2 = 4 option)
        00,01,10,11
    + công suất
    + dung tích động cơ (1 bit : 0/1)
Tạo các kiểu dữ liệu cụ thể để đại diện cho mỗi tính năng

-----------------------------------
Phép XOR ? 
- Ứng dụng : swap 2 số, viết hàm togglebit của chân GPIO 

- cách thông thường ? dùng temp -> đơn giản, dễ hiểu
- dùng XOR -> phức tạp , hiểu cách dữ liệu thao tác ở binary

xor
1 ^ 1 = 0
0 ^ 1 = 1

đặc điểm
+ tính triệt tiêu
a ^ a = 0
a ^ 0 = a 
+ tính giao hoán : 
(a ^ b) ^ b <=> a ^ (b ^ b)
(b ^ a) ^ a <=> b ^ (a ^ a)
-----------------------------------
a = a ^ b (1)
b = a ^ b (2)
a = a ^ b (3)

(2) <=> b = (1) ^ b 
    <=> b = a ^ (b ^ b)
    <=> b = a ^ 0 
    <=> b = a

(3) <=> a = a ^ b
    <=> a = (1) ^ a
    <=> a = a ^ b ^ a
    <=> a = (a ^ a) ^ b
    <=> a = 0 ^ b
    <=> a = b
------------------------------------------------------
Giao tiếp giữa 2 MCU 
- Yêu cầu ? đồng bộ phương thức truyền/nhận giữa 2 bên
    -> cấu hình truyền MSB/LSB trước ? 

TH1 : truyền MSB trước 
- truyền lần lượt từ MSB -> LSB bằng cách dịch phải
- nhận lần lượt MSB -> LSB và sắp xếp dữ liệu bằng cách dịch trái

for(i = 0 < 8) : 
    + i = 0 : datasend >> 7 : 0b 0000 0001
                              OR
                              0b 0000 0001 (mask)
                              0b 0000 0001 (bit)

              bit << 7      : 0b 1000 0000 
                              OR
                              0b 1000 0000 (receive)


















