#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum
{
    STACK_INIT_OK = 0, // Nếu cấp phát memory cho stack thành công
    STACK_FREE_OK,     // Nếu thu hồi memory thành công
    STACK_HANDLE_OK,   // Nếu push/pop/top bình thường
    STACK_FULL,        // stack đầy - ko thể push phần tử mới
    STACK_EMPTY,       // stack rỗng - chưa có data nào
    STACK_INVALID_ARG, // Lỗi truyền vào 1 giá trị ko hợp lệ
    STACK_MEMORY_ERROR // lỗi cấp phát/thu hồi/truy cập bộ nhớ ko hợp lệ
} StackStatus;

typedef struct
{
    int *items;
    int size;
    int top;
} Stack;

// Hàm in log tổng quát
#define STACK_LOG(data, errorcode)               \
    do                                           \
    {                                            \
        StackStatus _err = (errorcode);          \
        if (_err == STACK_INIT_OK)               \
            printf("Khởi tạo Stack thành công"); \
        else if (_err == STACK_FREE_OK)          \
            printf("Thu Hồi Stack thành công");  \
        else if (_err == STACK_FULL)             \
            printf("stack đầy");                 \
        else if (_err == STACK_EMPTY)            \
            printf("stack rỗng");                \
        else if (_err == STACK_INVALID_ARG)      \
            printf("Giá trị không hợp lệ");      \
        else if (_err == STACK_MEMORY_ERROR)     \
            printf("Lỗi truy cập bộ nhớ");       \
        else                                     \
        {                                        \
                                                 \
            printf("element = %d\taddress:%p",   \
                   *(const int *)data,           \
                   (const int *)data);           \
        }                                        \
        printf("\n");                            \
    } while (0)

StackStatus Stack_Init(Stack *stack, int size);
StackStatus Stack_Push(Stack *stack, int value);
StackStatus Stack_Pop(Stack *stack, int *outValue);
StackStatus Stack_Top(const Stack *stack, int *outValue);
StackStatus Stack_Free(Stack *stack);

#endif
