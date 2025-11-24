#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h> // size_t
#include <stdio.h>
typedef enum
{
    QUEUE_INIT_OK,  
    QUEUE_FREE_OK,
    QUEUE_HANDLE_OK,
    QUEUE_FULL,
    QUEUE_EMPTY,
    QUEUE_INVALID_ARG,
    QUEUE_MEMORY_ERROR
} QueueStatus;

typedef struct
{
    int *items;
    int size;
    int front;
    int rear;
} Queue;

// Hàm in log tổng quát
#define QUEUE_LOG(data, errorcode)               \
    do                                           \
    {                                            \
        QueueStatus _err = (errorcode);          \
        if (_err == QUEUE_INIT_OK)               \
            printf("Khởi tạo QUEUE thành công"); \
        else if (_err == QUEUE_FREE_OK)          \
            printf("Thu Hồi QUEUE thành công");  \
        else if (_err == QUEUE_FULL)             \
            printf("QUEUE đầy");                 \
        else if (_err == QUEUE_EMPTY)            \
            printf("QUEUE rỗng");                \
        else if (_err == QUEUE_INVALID_ARG)      \
            printf("Giá trị không hợp lệ");      \
        else if (_err == QUEUE_MEMORY_ERROR)     \
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

/* API */
QueueStatus Queue_Init(Queue *q, size_t size);
QueueStatus Queue_Enqueue(Queue *q, int data);
QueueStatus Queue_Dequeue(Queue *q, int *out);
QueueStatus Queue_Front(const Queue *q, int *out);
QueueStatus Queue_Rear(const Queue *q, int *out);
QueueStatus Queue_Free(Queue *q);

#endif
