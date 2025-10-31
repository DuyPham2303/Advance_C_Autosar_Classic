#include <stdio.h>
// xác định số phần tử của array
/*
    + kích thước array : sizeof(array)
    + kích thước phần tử :
    => số phần tử =  sizeof(array) / kích thước phần tử
*/

#define print(...) __VA_ARGS__

#define SUM(...)                                           \
    int arr[] = {__VA_ARGS__};                             \
    int i = 0;                                             \
    int tong = 0;                                          \
    for (int i = 0; i < sizeof(arr) / sizeof(arr[i]); i++) \
    {                                                      \
        tong += arr[i];                                    \
    }                                                      \
    printf("tong:%d\n", tong);

int main()
{
    print(12,34,53,12);

    //SUM(1, 2, 3, 4, 5);
    return 0;
}
