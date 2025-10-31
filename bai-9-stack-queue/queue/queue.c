#include "queue.h"

//Khởi tạo hàng đợi 
void Init_Queue(Queue *queue, int size){
    queue->items = (int*)malloc(sizeof(int) * size);
    if(queue->items == NULL){
        printf("vùng nhớ cấp phát không đủ");
        return;
    }
    queue->size = size;
    queue->front = queue->rear = -1;
}

//kiểm tra hàng đợi đầy
bool Isfull(Queue queue){
    return (queue.front == ((queue.rear + 1) % queue.size)); //(queue.rear == queue.size - 1);
}

//kiểm tra hàng đợi rỗng 
bool isEmpty(Queue queue){
    return (queue.front == QUEUE_EMPTY);
}

//thêm phần tử vào hàng đợi 
void enqueue(Queue *queue, int data){
    if(Isfull(*queue)){
        printf("queue đầy\n");
        return;
    }
    else{
        if(isEmpty(*queue)) queue->front = queue->rear = 0;
        else                queue->rear = (queue->rear + 1) % queue->size; //queue->rear++;
        queue->items[queue->rear] = data;
        printf("enqueue %d\tadd: %p\n",queue->items[queue->rear],&queue->items[queue->rear]);
    }
}

//xóa phần tử khỏi hàng đợi
void dequeue(Queue *queue){
    if(isEmpty(*queue)){
        printf("queue rỗng\n");
        return;
    }
    else{
        //printf("dequeue %d\tadd: %p\n",queue->items[queue->front],&queue->items[queue->front]);
        queue->items[queue->front] = 0;
        if(queue->front == queue->rear) queue->front = queue->rear = -1;
        else                            queue->front++; //queue->front = (queue->front + 1) % queue->size; 
    }
}

//lấy phần tử đầu hàng đợi
int getFront(Queue queue){
    return isEmpty(queue) ? QUEUE_EMPTY : queue.items[queue.front];
}
//lấy phần tử cuối hàng đợi
int getRear(Queue queue){
    return isEmpty(queue) ? QUEUE_EMPTY : queue.items[queue.rear];
}

//in toàn bộ phần tử hàng đợi
void printQueue(Queue queue){
    if(isEmpty(queue)){
        printf("queue rỗng\n");
        return;
    }
    else{
        int i = queue.front;
        printf("queue: ");
        while(1){
            printf("%d\t",queue.items[i]);
            if(i == queue.rear) break;
            i = (i + 1) % queue.size; //i++;
        }
        printf("\n");
    }
}

//giải phóng hàng đợi
void freeQueue(Queue *queue){
    if(queue->items != NULL){
        free(queue->items);
        queue->items = NULL;
    }
    queue->front = queue->rear = -1;
    queue->size = 0;
}