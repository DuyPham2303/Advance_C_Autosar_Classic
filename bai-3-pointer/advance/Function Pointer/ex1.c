#include <stdio.h>
void Tong(int a, int b)
{
    printf("%d + %d = %d\n", a, b, a + b);
}
void Hieu(int a, int b)
{
    printf("%d - %d = %d\n", a, b, a - b);
}
void Tich(int a, int b)
{
    printf("%d x %d = %d\n", a, b, a * b);
}
void Thuong(int a, int b)
{
    if (b != 0)
        printf("%d / %d = %.2f\n", a, b, (float)a / b);
    else
        printf("value of b is invalid\n");
}
void Operator(void (*op)(int, int), int a, int b)
{
    op(a, b);
}

int main()
{
    int a = 12, b = 2;
    /* cach 1 */
    // void (*func_ptr)(int,int) = NULL;
    // func_ptr = Tong;
    // func_ptr(a,b);

    // func_ptr = Hieu;
    // func_ptr(a,b);

    // func_ptr = Tich;
    // func_ptr(a,b);

    // func_ptr = Hieu;
    // func_ptr(a,b);
    /* cach 2 */
    // Operator(Tong, a, b);
    // Operator(Hieu, a, b);
    // Operator(Tich, a, b);
    // Operator(Thuong, a, b);
    /* cach 3 */

    void (*funcArr[])(int,int) = {Tong,Hieu,Tich,Thuong};
    funcArr[0](a,b);
    funcArr[1](a,b);
    funcArr[2](a,b);
    funcArr[3](a,b);
}