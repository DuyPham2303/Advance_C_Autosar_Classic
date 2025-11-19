#include"stack.h"

//hàm khởi tạo bộ nhớ cho ngăn xếp
StackStatus Init_Stack(Stack* stack,int size){
    stack->items = (int*)malloc(sizeof(int) * size);
    if(stack->items == NULL){
        printf("Heap Allocation failed\n");
        return STACK_E_NOT_OK;
    }
    stack->size = size;
    stack->top = -1;
    return STACK_E_OK;
}

//hàm kiểm tra stack đầy
StackStatus Isfull(const Stack* stack){
    return (stack->top == stack->size - 1);
}

//hàm kiểm tra stack rỗng
StackStatus IsEmpty(const Stack* stack){
    return (stack->top == -1);
}

//hàm thêm phần tử vào stack
StackStatus Push(Stack* stack,int data){
    if(Isfull(stack)){
        printf("stack đầy");
        return STACK_E_NOT_OK;
    }
    printf("thêm %d\n",data);
    stack->items[++stack->top] = data;  //tang8 top -> gán data
    return STACK_E_OK;
}

//hàm xóa phần tử ở đỉnh stack
StackStatus Pop(Stack* stack,int* outdata){
    if(IsEmpty(stack)){
        printf("stack rỗng");
        *outdata = -1;
        return STACK_E_NOT_OK;
    }
    *outdata = stack->items[stack->top--];  //trả về -> giảm top
    return STACK_E_OK;
}

//đọc / lấy phần tử ở đỉnh stack
StackStatus Top(const Stack* stack,int* data){
    if(IsEmpty(stack)){ 
        *data = -1;
        return STACK_E_NOT_OK;
    }
    *data = stack->items[stack->top];
    return STACK_E_OK;
}


//hàm giải phóng stack
StackStatus FreeStack(Stack* stack){
    //tránh gọi stack 2 lần sau khi đã free
    if(stack == NULL) return STACK_E_NOT_OK;

    if(stack->items != NULL){
        free(stack->items);
        stack->items = NULL;
    }
    stack->size = 0;
    stack->top = -1;
    return STACK_E_OK;
}

