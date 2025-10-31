#include <stdio.h>
int main(){
    printf("hello world");
}

/* 
    - preprocessor : gcc -E main.c -o main.i
    - compiler     : gcc -S main.i -o main.s
    - assembler    : gcc -c main.s -o main.o
    - linker       : gcc main.o -o main.exe
*/