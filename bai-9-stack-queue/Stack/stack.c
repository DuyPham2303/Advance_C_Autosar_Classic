#include"stack.h"

//hàm khởi tạo bộ nhớ cho ngăn xếp
void Init_Stack(Stack* stack,int size){
    stack->items = (int*)malloc(sizeof(int) * size);
    stack->size = size;
    stack->top = -1;
}

//hàm kiểm tra stack đầy
bool Isfull(Stack stack){
    return stack.top == stack.size - 1 ? true : false;
}

//hàm kiểm tra stack rỗng
bool IsEmpty(Stack stack){
    return stack.top == STACK_EMPTY ? true : false;
}

//hàm thêm phần tử vào stack
void Push(Stack* stack,int data){
    if(!Isfull(*stack)){
        stack->top++;
        stack->items[stack->top] = data;
        printf("thêm %d\n",stack->items[stack->top]);
    }
    else{
        printf("stack đầy");
    }
}

//hàm xóa phần tử ở đỉnh stack
int Pop(Stack* stack){
    if(!IsEmpty(*stack)){
        int data = stack->items[stack->top];
        stack->items[stack->top--];
        return data;
    }
    else{
        printf("stack rỗng");
        return STACK_EMPTY;
    }
}

//đọc / lấy phần tử ở đỉnh stack
int Top(Stack stack){
    if(IsEmpty(stack)) return -1;
    else{
        return stack.items[stack.top];
    }
}


//hàm giải phóng stack
void FreeStack(Stack* stack){
    if(stack->items != NULL){
        free(stack->items);
        stack->items = NULL;
    }
    stack->size = 0;
    stack->top = -1;
}

