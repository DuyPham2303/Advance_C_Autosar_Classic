#include <stdio.h>
#include <stdarg.h>

int sum(int count,...)
{
    va_list list;

    va_start(list, count);  

    int sum = 0;            
    
    for(int i = 0 ; i < count ; i++)
    {
        sum += va_arg(list,int);  
    }
    va_end(list); 
    return sum;
}

int main()
{
    int result = sum(5,1,2,3,4);
    printf("result = %d",result);
    return 0;
}