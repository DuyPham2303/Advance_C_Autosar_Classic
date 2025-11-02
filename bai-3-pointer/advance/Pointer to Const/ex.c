#include <stdio.h>
/* 
    - có thể trỏ tới bất kỳ địa chỉ nào
    - giá trị tại địa chỉ mà nó trỏ tới là cố định
*/
const char* str = "hello world"; //ro data
int main(){
    int a = 23;
    const int* ptr = &a;
    *ptr = 12;
    //int b = 11;
    //ptr = &b;
    str = 'h';
}