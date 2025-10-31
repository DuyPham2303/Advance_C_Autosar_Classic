#include <stdio.h>

#define A 123

/* 
    label1:

    goto label2;

    label2:
        //cau lenh 11
        //cau lenh 12
*/


//PC : program counter -> thanh ghi trỏ tới câu lệnh thực thi hiện tại
int main()
{
    int i = 0;            
    // int running = 0;   
    // while (running)
    // {
    //     if (i > 5)
    //     {
    //         running = 0;
    //     }
    //     else
    //     {
    //         printf("i = %d\n", i);
    //         i++;
    //     }
    // }

    start:
    if (i > 5) goto end;

    printf("i = %d\n", i);
    i++;

    goto start;

    end: 
   
    return 0;
}