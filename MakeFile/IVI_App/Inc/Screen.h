#ifndef SCREEN_H
#define SCREEN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Cấu trúc ngăn xếp để lưu trữ màn hình 
typedef struct{
    char** list_screen; //mảng con trỏ lưu trữ chuỗi tên của màn hình
    int size;       //max screen lưu trữ 
    int top;        //chỉ số truy cập màn hình hiện tại (ở đầu ngăn xếp)
}Screen_Stack;

void Init_ListScreen(Screen_Stack* screen,int max_screen);

bool IsScreenfull(Screen_Stack screen);

bool IsScreenEmpty(Screen_Stack screen);

void map_screen(const char* screen);

void show_current_screen(Screen_Stack screen_stack);

void push_screen(Screen_Stack* screen_stack,const char* screen_name);

void pop_screen(Screen_Stack* screen_stack);


#endif