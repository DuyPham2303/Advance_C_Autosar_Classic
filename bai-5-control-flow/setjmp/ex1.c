#include <stdio.h>
#include <setjmp.h>

jmp_buf buf;

int exception = 0;

void func2()
{
    printf("This is function 2\n");
    longjmp(buf, 2);
}

void func3()
{
    printf("This is function 3\n");
    longjmp(buf, 3);
}

void func1()
{
    exception = setjmp(buf);
    if (exception == 0)
    {
        printf("This is function 1\n");
        printf("exception = %d\n", exception);
        func2();
    }
    else if (exception == 2)
    {
        printf("exception = %d\n", exception);
        func3();
    }
    else if (exception == 3)
    {
        printf("exception = %d\n", exception);
    }
}
int arr[5]; //arr[4]
int* p = NULL;
int main()
{
    arr[5] = 12; //truy cập ngoài phạm vi mảng
    //printf("arr[5] = %d",arr[5]);
    printf("*p = %d",*p);
    //func1();
    //int a = 3/0;
    //printf("a = %d",a);
    return 0;
}


