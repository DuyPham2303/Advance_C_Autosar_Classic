#include <stdio.h>


void swap(int *pa, int *pb) //0xb1 : 0x04 , 0xb8 : 0x08
{
    int tmp = *pa;   //temp = 23 
    *pa = *pb;       //*a = 12 (0x04)
    *pb = tmp;       //*b = 23 (0x08)
} 


void swap(int a,int b){ //0xa0 : 23 , 0xa8 : 12
    int tmp = a; //temp = 23 
    a = b;       //a = 12 (0xa0)
    b = tmp;     //b = 23 (0xa8)
}

int main(){
    int a = 23; // 0x04
    int b = 12; // 0x08

    swap(&a,&b); //0x04,0x08

    printf("a:%d\tb:%d\n",a,b); 
    return 0;
}