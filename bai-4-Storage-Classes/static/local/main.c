#include "stdio.h"
int* pa = NULL;
void test(){
    static int a = 0; //0x01 - 0x04
    pa = &a;
    printf("a = %d\n",++a); // a = 3
}
int main(){
    test(); // a = 1
    test(); // a = 2
    test(); // a = 3
    *pa = 23;
    test();
}