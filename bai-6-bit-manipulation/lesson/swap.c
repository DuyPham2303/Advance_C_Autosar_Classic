#include <stdio.h>


void Swap(int *a, int *b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

int main()
{
    int a = 21, b = 25;
    printf("a = %d\tb = %d\n", a, b);
    Swap(&a, &b);
    printf("a = %d\tb = %d\n", a, b);
    return 0;
}