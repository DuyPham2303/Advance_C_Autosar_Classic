#include <stdio.h>

//text segment -> .text , .rdata hoặc .rodata (sections)

const char* str = "hello"; //.rdata (read-only) 
                           // str -> .data thuộc data segment 
                     //.data (section) -> data segment 
const int a = 21;    //.rdata (section)

int data_1 = 21;    //.data 

static int data_2 = 21; //.data 

//unintialized data 
int bss_1 = 0;  //.bss 
int bss_2;      //.bss

static int bss_3 = 0;
static int bss_4;

char str1[] = "hello";  //"hello" -> .rdata section (text segment)
                        //str1 -> 'h','e','l','l','o'
const char str2[] = "hello";  //  str2 , "hello" -> .rdata 

char* str3[] = {"hello","world","ute"}; //"hello","world","ute" -> .rdata 
                                        //str3 -> .data 

int main(){
    static int data_3 = 10; //.data 

    data_3 = 32;
    //a = 28;
    //str[1] = '2';

    str = "world";
    printf("%s",str);
    return 0;
}