#include "queue.h"

void Display_Queue(const Queue *q)
{
    int i = q->front;
    printf("display queue\n");
    while (1)
    {
        printf("value %d\taddress %p\n", q->items[i], &q->items[i]);
        if (i == q->rear)
            break;
        /* linear queue */
        // i++;
        /* circular queue */
        i = (i + 1) % q->size;
    }
    printf("\n");
}

int giaithua(int n){ 
    Queue queue;

    Queue_Init(&queue,n);

    for(int i = n ; i >= 1 ; i--)     //n(n - 1)(n - 2)....1
        Queue_Enqueue(&queue,i);
    
    int result = 1;
    for(int i = 1; i <= n ;i++){
        int outdata = 0;
        Queue_Dequeue(&queue,&outdata);
        printf("%d",outdata);

        if(i < n)
            printf("*");
        result *= outdata;
    }
    return result;
}

int main()
{

    int result = giaithua(5);
    printf(" = %d",result);
    return 0;

    Queue queue;
    int size = 5;

    QUEUE_LOG(NULL, Queue_Init(&queue, size));

    for (int i = 0; i < size; i++)
        // QUEUE_LOG(&queue.items[queue.rear],Queue_Enqueue(&queue,i));
        Queue_Enqueue(&queue, i);

    // QUEUE_LOG(&queue.items[queue.rear],Queue_Enqueue(&queue,10));

    printf("after enqueue 1st\n");
    Display_Queue(&queue);

    int outdata = 0;
    // QUEUE_LOG(&outdata,Queue_Dequeue(&queue,&outdata));
    // QUEUE_LOG(&outdata,Queue_Dequeue(&queue,&outdata));
    // QUEUE_LOG(&outdata,Queue_Dequeue(&queue,&outdata));
    Queue_Dequeue(&queue, &outdata);
    Queue_Dequeue(&queue, &outdata);
    Queue_Dequeue(&queue, &outdata);

    printf("after dequeue\n");
    Display_Queue(&queue);

    Queue_Enqueue(&queue, 7);
    Queue_Enqueue(&queue, 8);
    Queue_Enqueue(&queue, 9);

    printf("after enqueue 2nd\n");
    Display_Queue(&queue);
    return 0;
}