#include "queue.h"
#include <stdlib.h>

QueueStatus Queue_Init(Queue *q, size_t size)
{
    if (q == NULL || size <= 0)
    {
        return QUEUE_INVALID_ARG;
    }

    q->items = (int *)malloc(size * sizeof(int));
    if (q->items == NULL)
    {
        return QUEUE_MEMORY_ERROR;
    }

    q->size = size;
    q->front = -1;
    q->rear  = -1;

    return QUEUE_HANDLE_OK;
}

bool Queue_IsEmpty(const Queue *q)
{
    if (q == NULL) return true;
    return (q->front == -1);
}

bool Queue_IsFull(const Queue *q)
{
    if (q == NULL) return false;
    return (q->front == ((q->rear + 1) % q->size));
}

QueueStatus Queue_Enqueue(Queue *q, int data)
{
    if (q == NULL) return QUEUE_INVALID_ARG;
    if (Queue_IsFull(q)) return QUEUE_FULL;

    // Case queue empty → reset front, rear
    if (Queue_IsEmpty(q))
    {
        q->front = q->rear = 0;
    }
    else
    {
        q->rear = (q->rear + 1) % q->size;
    }

    q->items[q->rear] = data;
    return QUEUE_HANDLE_OK;
}

QueueStatus Queue_Dequeue(Queue *q, int *out)
{
    if (q == NULL || out == NULL) return QUEUE_INVALID_ARG;
    if (Queue_IsEmpty(q)) return QUEUE_EMPTY;

    *out = q->items[q->front];

    // Nếu chỉ có 1 phần tử → reset lại queue
    if (q->front == q->rear)
    {
        q->front = q->rear = -1;
    }
    else
    {
        q->front = (q->front + 1) % q->size;
    }

    return QUEUE_HANDLE_OK;
}

QueueStatus Queue_Front(const Queue *q, int *out)
{
    if (q == NULL || out == NULL) return QUEUE_INVALID_ARG;
    if (Queue_IsEmpty(q)) return QUEUE_EMPTY;

    *out = q->items[q->front];
    return QUEUE_HANDLE_OK;
}

QueueStatus Queue_Rear(const Queue *q, int *out)
{
    if (q == NULL || out == NULL) return QUEUE_INVALID_ARG;
    if (Queue_IsEmpty(q)) return QUEUE_EMPTY;

    *out = q->items[q->rear];
    return QUEUE_HANDLE_OK;
}

QueueStatus Queue_Free(Queue *q)
{
    if (q == NULL) return QUEUE_INVALID_ARG;

    if (q->items != NULL)
    {
        free(q->items);
        q->items = NULL;
    }

    q->front = q->rear = -1;
    q->size = 0;
    return QUEUE_FREE_OK;
}

void Display_Queue(const Queue* q){
    if(Queue_IsEmpty(q))
        printf("queue đầy\n");
    int i = q->front; 
    printf("display queue\n"); 
    while(1){ 
        printf("%d\taddress : %p\n",q->items[i],&q->items[i]); 
        if(i == q->rear) 
            break; 
        i = (i + 1) % q->size; 
        //i++;
    } 
    printf("\n");
}