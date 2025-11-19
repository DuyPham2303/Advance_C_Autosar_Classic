#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef bool StackStatus;

#define STACK_E_OK        ((StackStatus)true)
#define STACK_E_NOT_OK    ((StackStatus)false)

typedef struct{
    int* items;
    int size;
    int top;
}Stack;

//hàm khởi tạo bộ nhớ cho ngăn xếp
StackStatus Init_Stack(Stack* stack,int size);

//hàm kiểm tra stack đầy
StackStatus Isfull(const Stack* stack);

//hàm kiểm tra stack rỗng
StackStatus IsEmpty(const Stack* stack);

//hàm thêm phần tử vào stack
StackStatus Push(Stack* stack,int indata);

//hàm xóa phần tử ở đỉnh stack
StackStatus Pop(Stack* stack,int* outdata);

//đọc / lấy phần tử ở đỉnh stack
StackStatus Top(const Stack* stack,int *data);

//hàm giải phóng stack
StackStatus FreeStack(Stack* stack);


#endif