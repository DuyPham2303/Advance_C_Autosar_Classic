#include <stdio.h>

int data = 21;
int* ptr = &data; // .data 

int test(int a , int b){      //0x100
    int result;               
    result = a + b;             

    const int c = 21;
    
    ptr = &c;

    *ptr = 29;
    printf("%d",*ptr);  

    //printf("add a:%p\nadd b:%p\nadd result :%p\n",&a,&b,&result);
    return result;   
}

int main(){
    int a = 21;                     //0x01
    int b = 29;                     //0x02
    int c = a + b;                  //0x03
    int result = test(c,a);         //0x04
    printf("result = %d",result);
    return 0;
}