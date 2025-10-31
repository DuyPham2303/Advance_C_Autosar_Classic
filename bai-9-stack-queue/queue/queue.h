#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define QUEUE_EMPTY -1

//Cấu trúc tổ chức hàng đợi
typedef struct
{
    int *items;
    int size;
    int front;
    int rear;
} Queue;

//Khởi tạo hàng đợi 
void Init_Queue(Queue *queue, int size);

//kiểm tra hàng đợi đầy
bool Isfull(Queue queue);

//kiểm tra hàng đợi rỗng 
bool isEmpty(Queue queue);

//thêm phần tử vào hàng đợi 
void enqueue(Queue *queue, int data);

//xóa phần tử khỏi hàng đợi
void dequeue(Queue *queue);

//lấy phần tử đầu hàng đợi
int getFront(Queue queue);

//lấy phần tử cuối hàng đợi
int getRear(Queue queue);

//in toàn bộ phần tử hàng đợi
void printQueue(Queue queue);

//giải phóng hàng đợi
void freeQueue(Queue *queue);