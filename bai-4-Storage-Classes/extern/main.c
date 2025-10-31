#include <stdio.h>
#include "test1.h"
/* 
    - nhắc cho compiler biết 1 biến/hàm đã được định nghĩa
    - chỉ sử dụng cho biến/hàm global
    - chỉ được phép khai báo , ko cho phép định nghĩa
*/
int main(){
    test1();
    a = 21;
    printf("a = %d",a);
    return 0;
}