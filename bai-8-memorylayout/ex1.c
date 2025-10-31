#include <stdio.h>
/* 
    + Text:
    - chứa mã máy -> lệnh thực thi
    - quyền truy cập : read only -> ko thể write
    - chứa biến const , chuỗi hằng -> "hello" (MacOs)
    + Data:
    - chứa biến global khác 0 
    - chứa biến static (global,local) khác 0
    - quyền truy cập : read/write
    - chứa biến const , chuỗi hằng (Window)
    - vùng nhớ thu hồi khi chương trình kết thúc
    + Bss:
    - chứa biến global / static khởi tạo = 0 hoặc ko khởi tạo
    - quyền truy cập: đọc/ghi
    - vùng nhớ thu hồi khi chương trình kết thúc
*/

const int a = 0;    //text -> macOs , rodata -> Window

// "hello" -> text (mac0s), "hello" -> rodata (window)
//str -> rodata
const char* str = "hello"; 
/* 
    "hello" -> rodata(0x00 - 0x04) -> window
    str1[] -> rodata (0xa1 - 0xa5)
*/
const char str1[] = "hello";  

int data1 = 1;  // data 

static int data2 = 2; //data


int static_data1;   //bss

int static_data2 = 0;   //bss

static int static_data3; //bss

typedef struct{
    int a;
    int b;
}data;

int main(){
    data dt = {.a = 0,.b = 0};  //bss
    data dt1 = {.a = 1,.b = 2}; //data

    // static int data3 = 3; //data
    // str[2] = '2';
    // str1[2] = '2';
    // printf("%s",str);
    return 0;
}