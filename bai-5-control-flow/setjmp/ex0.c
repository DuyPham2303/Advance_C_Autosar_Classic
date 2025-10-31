#include <stdio.h>
#include <setjmp.h>

jmp_buf buf;

int exception;

int main(int argc, char const *argv[])
{
    exception = setjmp(buf);
    if (exception == 0)
    {
        printf("This is function 1\n");
        printf("exception = %d\n", exception);
        longjmp(buf,2);
    }
    else if (exception == 2)
    {
        printf("exception = %d\n", exception); 
    }
    return 0;
}


