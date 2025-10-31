#include <stdio.h>
#include <string.h>
char* str[] = {"hello" , "123", "double" , "12.21"};
int main(){ 
    char** p = str; //&str -> 0x8c3 , p -> &str
    
    while(*p){
        static int i = 0;
        printf("%s\n",*p);
        p+=1;
    }
    return 0;
}