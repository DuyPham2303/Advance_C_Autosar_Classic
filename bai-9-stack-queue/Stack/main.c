#include "stack.h"

void Push_Stack(Stack* stack,int size){
    printf("Quá trình push\n");
    for(int i = 0 ; i < size; i++){
        STACK_LOG(&stack->items[stack->top],Stack_Push(stack,i));
    }
}
void Pop_Stack(Stack* stack,int size){
    printf("Quá trình pop\n");
    for(int i = 0 ; i < size; i++){
        int outdata = 0;
        Stack_Pop(stack,&outdata);
        printf("pop %d\n",outdata);
    }
}
void Top_Stack(const Stack* stack){
    int outdata = 0;
    STACK_LOG(&outdata,Stack_Top(stack,&outdata));
}
int main(){
    Stack stack;
    int size = 0;
    while(size <= 0){
        printf("nhap kich thuoc stack:");
        scanf("%d",&size);
        StackStatus status = Stack_Init(&stack,size);
        STACK_LOG(NULL,status);
    };

    do{
        static int choice;
        printf("1.push stack\n");
        printf("2.pop stack\n");
        printf("3.top stack\n");
        printf("4.free stack\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                Push_Stack(&stack,size);
                break;
            case 2:
                Pop_Stack(&stack,size);
                break;
            case 3:
                Top_Stack(&stack);
                break;
            case 4:
                STACK_LOG(&stack,Stack_Free(&stack));
                goto endprogram;
                break;
            default:
                break;
        }
    }while(1);
    endprogram:
    return 0;
}