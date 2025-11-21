#ifndef EVENT_H
#define EVENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//cấu trúc hàng đợi để lưu trữ sự kiện xử lý (do người user nhấn nút)
typedef struct{
    char** list_event ; //mảng con trỏ lưu trữ chuỗi sự kiện chở được xử lý
    int size;       //max events lưu trữ
    int rear;       //truy cập latest event 
    int front;      //truy cập earliest event 
}Event_queue;

void Init_ListEvent(Event_queue* event,int max_event);

void enqueue_event(Event_queue* event_queue,const char* event_name);

void process_event(Event_queue* event_queue);


#endif