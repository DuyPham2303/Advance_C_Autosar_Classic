#include <stdio.h>

int main(){
    printf("size of pointer to integer : %d\n",sizeof(char*));
    printf("size of pointer to integer : %d\n",sizeof(int*));
    printf("size of pointer to integer : %d\n",sizeof(float*));
    printf("size of pointer to integer : %d\n",sizeof(double*));
    return 0;
}

//x64 : 8byte
//x86 - 32 bite : 4byte (gcc)

//char* -> 1 byte
//double* -> 8 byte
//float* -> 4 byte