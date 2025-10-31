#include <stdint.h>
#include <stdio.h>

typedef struct Info
{
    char status;          
    unsigned int flags : 4; 
    unsigned int mode : 3;  
    unsigned int error : 1; 
    int *ptr;             
    char label[5];         
    double value;         
}Info;

int main()
{ 
     printf("sizeof(Info)          = %zu\n", sizeof(Info));
     printf("offset of (status)    = %zu\n", offsetof(Info, status));
     printf("offset of (ptr)       = %zu\n", offsetof(Info, ptr));
     printf("offset of (label)     = %zu\n", offsetof(Info, label));
     printf("offset of (value)     = %zu\n", offsetof(Info, value)); 
}