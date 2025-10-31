#include <stdio.h>

void single_ptr(char* ptr){ //ptr(0x12) -> 'h'
    ptr+=4; // ptr -> 'o'
    printf("%s\n",ptr); //
}
void double_ptr(char** pstr){   //pstr(0xab) = 0x01
    //*pstr;// *(0x01) -> 'h'  
    // printf("address of 'h' : %p\n",*pstr);
    // printf("%c",**pstr);
    *pstr+=4;
}
int main(){
    char* str = "hello world"; // str(0x01) -> 'h'

    //single_ptr(str);
    double_ptr(&str); //0x01

    printf("outside: %s",str);

}