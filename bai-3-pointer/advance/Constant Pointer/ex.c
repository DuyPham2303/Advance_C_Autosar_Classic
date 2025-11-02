#include <stdio.h>

int main(){
    /* 
        - trỏ tới 1 vùng địa chỉ duy nhất sau khi khởi tạo
        - giá trị tại vùng địa chỉ mà nó trỏ tới có thể thay đổi
    */
   int a = 12, b = 14;
   int* const p = &a;

   *p = 12;
   p = &b;

}