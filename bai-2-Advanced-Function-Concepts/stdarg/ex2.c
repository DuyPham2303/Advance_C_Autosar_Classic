#include <stdio.h>
#include <stdarg.h>

#define SUM(...) sum2(__VA_ARGS__,"abs")

int sum2(int count, ...)
{
    va_list args;

    va_list check;

    va_start(args, count);

    va_copy(check, args);

    int result = count;

    int value = 0;

    while ((va_arg(check, char *)) != (char *)"abs")
    {
        result += va_arg(args, int);
    }

    va_end(args); // giai phong con tro
    return result;
}
int main()
{
    int result = SUM(12,10,12,14);
    printf("result = %d",result);
    return 0;
}