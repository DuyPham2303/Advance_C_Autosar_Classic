#include <stdio.h>
/* 
    LIFO : Biến / tham số cục bộ được cấp phát cuối cùng -> thu hồi đầu tiên

*/
int test(int a ,int b){ //a(0a1 - 0xa3) , b(0xb4 - 0x0b7)
    int result = a + b; //result (0xb8 - 0xab)
    printf("address of a\t%p\n",&a);
    printf("address of b\t%p\n",&b);
    printf("address of result\t%p\n",&result);
    return result; 
}
int main(){
    int a = 1, b = 2; //a(0x01 - 0x03) ,b (0x04 - 0x7)
    int result = test(a,b); // copy a va b
    printf("result (outside test) :%d",result);
    return 0;
}