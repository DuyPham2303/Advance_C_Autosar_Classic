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
    printf("a = %d\nb = %d\n", a, b);
    Swap(&a, &b);
    printf("a = %d\nb = %d\n", a, b);
    return 0;
}