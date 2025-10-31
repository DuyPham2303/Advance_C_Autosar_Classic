#include "Event.h"
static int count = 0;

void Init_ListEvent(Event_queue* event,int max_event){
   event->list_event = (char**)malloc(max_event * sizeof(char*)); 
    if(event->list_event == NULL){
        printf("không đủ bộ nhớ cấp phát\n");
        return;
    }
   event->size = max_event;
   event->front = event->rear = -1;
}
bool IsEventFull(Event_queue event){
    return (event.front == ((event.rear + 1) % event.size));
}
bool IsEventEmpty(Event_queue event){
    return (event.front == -1);
}

void enqueue_event(Event_queue* event_queue,const char* event_name) {
    if (!IsEventFull(*event_queue)) {
        if(IsEventEmpty(*event_queue)){
            event_queue->front = event_queue->rear = 0;
        }
        else{
            event_queue->rear = (event_queue->rear + 1) % event_queue->size;
        }
        int i = event_queue->rear;

        //cấp phát heap cho phần tử hiện tại 
        event_queue->list_event[i] = (char*)malloc(strlen(event_name) + 1);

        //kiểm tra cấp phét hợp lệ
        if(event_queue->list_event[i] == NULL){
            printf("không đủ heap để lưu %s\n",event_name);
            return;
        }

        //sao chéo chuỗi vào heap
        strcpy(event_queue->list_event[i],event_name);

        //in ra chuỗi đã sao chép
        printf("Queued event: %s\n", event_queue->list_event[i]);
 
        
    } else {
        printf("Event queue full\n");
    }

    //đếm số lượng phần tử đã enqueue
    count++;
    if(count <= (*event_queue).size) printf("total Events : %d\n",count);
    else count = (*event_queue).size;
}

void process_event(Event_queue* event_queue) {
    if (!IsEventEmpty(*event_queue)) {
        printf("Processing event: %s\n", event_queue->list_event[event_queue->front]);

        free(event_queue->list_event[event_queue->front]);
        
        if(event_queue->front == event_queue->rear){
            event_queue->front = event_queue->rear = -1;
        }
        else{
            event_queue->front = (event_queue->front + 1) % event_queue->size;
        }
    } else {
        printf("No events to process\n");
    }

    //đếm số lượng phần tử còn lại sau khi dequeue
    count--;
    if(count >= 0)   printf("total Events : %d\n",count);
    else count = 0;
}