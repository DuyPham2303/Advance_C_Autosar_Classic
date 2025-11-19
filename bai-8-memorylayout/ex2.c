#include <stdio.h>

int data = 21;
int* ptr = &data; // .data 
int test(int a , int b){    
    int result;
    result = a + b;

    //int* ptr = NULL;

    //pointer -> bss/data/stack trỏ tới stack bình thường 

    const int c = 21;
    
    ptr = &c;

    *ptr = 29;
    printf("%d",*ptr);

    //printf("add a:%p\nadd b:%p\nadd result :%p\n",&a,&b,&result);
    return result;
}

int main(){
    int a = test(12,21);    //khi test được gọi -> return address sẽ được push main stack pointer 

    // a = test(32,21);

    // a = test(23,37);

    return 0;
}