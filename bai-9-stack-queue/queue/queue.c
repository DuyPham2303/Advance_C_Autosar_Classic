#include "queue.h"

static bool Queue_IsFull(const Queue* q){
    /* linear queue */
    //return (q->rear == q->size - 1); 
    /*circular queue*/
    return (q->front == ((q->rear + 1) % q->size)); 
}
static bool Queue_IsEmpty(const Queue* q){
    return (q->front == -1);
}
QueueStatus Queue_Init(Queue *q, size_t size){
    if(size <= 0) return QUEUE_INVALID_ARG;

    q->items = (int*)malloc(size * sizeof(int));
    if(q->items == NULL){
        return QUEUE_MEMORY_ERROR;
    }

    q->size = size;
    q->front = q->rear = -1;
    return QUEUE_INIT_OK;
}
QueueStatus Queue_Enqueue(Queue *q, int data){
    if(q == NULL) return QUEUE_INVALID_ARG;

    if(Queue_IsFull(q)) return QUEUE_FULL;

    if(Queue_IsEmpty(q)){
        q->front = q->rear = 0;
    }
    else{
        /* linear queue */
        //q->rear++;
        /* circular queue */
        q->rear = (q->rear + 1) % q->size;
    }
    q->items[q->rear] = data;
    return QUEUE_HANDLE_OK;

}
QueueStatus Queue_Dequeue(Queue *q, int *out){
    if(q == NULL || out == NULL) return QUEUE_INVALID_ARG;
    if(Queue_IsEmpty(q)) return QUEUE_EMPTY;

    *out = q->items[q->front];

    if(q->front == q->rear){
        q->front = q->rear = -1;
    }
    else{
        /* linear queue */
        //q->front++;
        /* circular queue */
        q->front = (q->front + 1) % q->size;
    }
    return QUEUE_HANDLE_OK;
}
QueueStatus Queue_Front(const Queue *q, int *out){
    if(q == NULL || out == NULL) return QUEUE_INVALID_ARG;
    if(Queue_IsEmpty(q)) return QUEUE_EMPTY;

    *out = q->items[q->front];
    return QUEUE_HANDLE_OK;
}
QueueStatus Queue_Rear(const Queue *q, int *out){
    if(q == NULL || out == NULL) return QUEUE_INVALID_ARG;
    if(Queue_IsEmpty(q)) return QUEUE_EMPTY;

    *out = q->items[q->rear];
    return QUEUE_HANDLE_OK;
}
QueueStatus Queue_Free(Queue *q){
    if(q == NULL) return QUEUE_INVALID_ARG;

    if ((q->items != NULL))
    {
        free(q->items);
        q->items = NULL;
    }

    q->front = q->rear = -1;
    q->size = 0;
    return QUEUE_FREE_OK;
    
}