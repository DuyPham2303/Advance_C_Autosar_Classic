#include <stdio.h>

#define print(...) __VA_ARGS__

//sizeof(arr) = 20 byte

//int arr[5]; // size of arr = 5 phan tu * 4 byte

#define SUM(...)               \
    int arr[] = {__VA_ARGS__,0}; \
    int index = 0;             \
    int tong = 0;              \
    for(; index < sizeof(arr) / sizeof(arr[0]) ; index++)\
    {                          \
        tong += arr[index];  \
    }                          \
    printf("tong = %d\n", tong);

int main()
{
    SUM(1, 2, 3, 4, 5, 6);
    return 0;
}
