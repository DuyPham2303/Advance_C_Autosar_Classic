#include<stdio.h>
#include<stdarg.h>

//short , char -> mở rộng int hoặc char* 
//float -> mở rộng double

void test(int count, ...) 
{
    va_list args; //args = 5, 1, 2, "hello", 'a', 3.5

    va_start(args,count); // args = 1, 2, "hello", 'a', 3.5

    printf("value 1 : %d\n",va_arg(args,int));
    printf("value 2 : %d\n",va_arg(args,int));
    printf("value 3 : %s\n",va_arg(args,char*));
    printf("value 4 : %c\n",va_arg(args,char*));
    printf("value 5 : %lf\n",va_arg(args,double));

    va_end(args);
}

int main(void)
{
    test(5, 1, 2, "hello", 'a', 3.5);
    
    return 0;
}
