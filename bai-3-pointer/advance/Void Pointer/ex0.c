#include <stdio.h>
#include <stdint.h>
#include <string.h>
/*
    - void pointer
    - Null pointer
    - const pointer
    - pointer to const
    - function pointer
    - pointer to pointer (double pointer)
*/
/* 

*/
char* pstr[] = {"hello","world","my name","is Duy"};
int main()
{
    int a = 12;
    char b = 's';
    float c = 12.32;
    double d = 122333.12;
    char* str = "hello  world"; //'\0'

    //void *ptr = &a;
    void* ptr[] = {&a,&b,&c,&d,str};
    //printf("a = %d\n", *(int *)ptr[0]);

    //ptr = &b;
    //printf("b = %c\n", *(char *)ptr[1]);

    //ptr = &c;
    //printf("c = %.2f\n", *(float *)ptr[2]);

    //ptr = &d;
    //printf("a = %lf\n", *(double *)ptr[3]);

    //printf("str = %s\n",(char*)ptr[4]);

    // for(int i = 0 ; i < sizeof(str) ; i++){
    //      printf("str[%d] = %c\n",i,*((char*)ptr[4] + i));
    // }

    printf("size of pstr : %d\n",sizeof(pstr));
    // for(int i = 0 ; i < sizeof(pstr) / sizeof(pstr[0]); i++){
    //     printf("str[%d] : %s\n",i,pstr[i]);
    // }
    int len = strlen(pstr[2]);
    for(int i = 0 ; i < len  ; i++){
        printf("%c",*(pstr[2] + i));
    }



    return 0;
}