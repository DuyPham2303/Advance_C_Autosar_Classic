# include "queue.h"

int giaithua(int n){
    Queue queue;
    int size = 5;
    Queue_Init(&queue,size);

    for(int i = 1 ; i <= size ; i++)
        Queue_Enqueue(&queue,i);

    int result = 1;
    for(int i = 1 ; i <= size ; i++){
        int outdata;
        Queue_Dequeue(&queue,&outdata);
        printf("%d",outdata);
        if(i < size) 
            printf("*");
        result *= outdata;
    }
    return result;
}
int main(){
    
    printf("= %d",giaithua(5));
    return 0;
    Queue queue;

    Queue_Init(&queue,5);

    for(int i = 0 ; i < 5 ; i++){
        QUEUE_LOG(&queue.items[queue.rear],Queue_Enqueue(&queue,i));
    }

    QUEUE_LOG(&queue.items[queue.rear],Queue_Enqueue(&queue,21));
    
    printf("after enqueue\n");
    Display_Queue(&queue);
    
    int outdata;
    
    QUEUE_LOG(&outdata,Queue_Dequeue(&queue,&outdata));
    QUEUE_LOG(&outdata,Queue_Dequeue(&queue,&outdata));
    printf("after dequeue\n");
    Display_Queue(&queue);
    
    printf("after enqueue\n");
    QUEUE_LOG(&queue.items[queue.rear],Queue_Enqueue(&queue,41));
    QUEUE_LOG(&queue.items[queue.rear],Queue_Enqueue(&queue,29));
    Display_Queue(&queue);


    return 0;
}