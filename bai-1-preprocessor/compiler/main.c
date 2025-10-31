//chỉ thị thay thế tệp 
#include <stdio.h>
#include "ex.h"

//chỉ thị thay thế văn bản 
#define SIZE 5

int size = 5;

int arr[SIZE]; // compiler mở rộng SIZE và tìm thấy đoạn văn bản thay thế 

#define CREATE_FUNCTION(name, STR) \
    void name()                    \
    {                              \
        printf(STR);               \
    }

CREATE_FUNCTION(func1, "this is function 1\n");
CREATE_FUNCTION(func2, "this is function 2\n");
CREATE_FUNCTION(func3, "this is function 3\n");


// chỉ thị biên dịch có điều kiện : #ifndef #ifdef #endif #error #if #elif #else

#undef SIZE

#ifndef SIZE
    #define SIZE 10
#endif //được dùng để kết thúc việc xử lý các chỉ thị khác

int main()
{
    
    SIZE;
    demo();
    func1();
    func2();
    func3();
    for(int i = 0 ; i < SIZE ; i++){
        arr[i] = i + 2;
        printf("arr[%d]:%d\n",i,arr[i]);
    }
    return 0;
}
