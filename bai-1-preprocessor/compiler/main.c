// chỉ thị bao hàm tệp
#include <stdio.h>
#include "ex.h"

// chỉ thị thay thế văn bản
#define SIZE 5

int size = 5;

int arr[SIZE]; // compiler mở rộng SIZE và tìm thấy đoạn văn bản thay thế

#define CREATE_FUNC(name,str)   \
    void name(){                \
        printf(str);            \
    }                           \

CREATE_FUNC(func1,"this is func1\n");
CREATE_FUNC(func2,"this is func2\n");
CREATE_FUNC(func3,"this is func3\n");

#undef SIZE

#ifndef SIZE
#define SIZE 10
#endif

int main()
{
    func1();
    func2();
    func3();
    demo();

    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = i + 2;
        printf("arr[%d]:%d\n", i, arr[i]);
    }
    return 0;
}
