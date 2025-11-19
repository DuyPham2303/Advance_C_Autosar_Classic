#include "stack.h"

int tinhgiaithua(int n){
    // if(n == 1)return 1;
    // return n*tinhgiaithua(n - 1);

    Stack stack1;

    (void)Init_Stack(&stack1,n);

    for(int i = n ; i > 0 ;i--)
        (void)Push(&stack1,i);    //(n - 1)(n - 2)(n - 3)...1
    
    int result = 1;

    for(int i = 0 ; i < n ;i++){
        static int out = 0;
        (void)Pop(&stack1,&out); 
        result *= out;
    }
    return result;
}
int main()
{
    int n = 5;
    printf("giai thua cua %d = %d",n,tinhgiaithua(n));
    
    return 0;
    Stack stack1;
    int size = 5;           //kích thước của stack
    (void)Init_Stack(&stack1, size); //khởi tạo giá trị ban đầu cho stack

    //in và Lưu giá trị vào stack 
    for (int i = 0; i < size; i++)
    {
        (void)Push(&stack1, i + 2);
        printf("element: %d -> add:%p\n", stack1.items[i], &stack1.items[i]);
    }

    
    //In và lấy từng phần tử ra khỏi stack    
    for (int i = size - 1 ; i >= -1; i--)
    {
        static int current_pop;
        (void)Pop(&stack1,&current_pop);
        printf("\ntop element: %d -> add:%p\n", current_pop, &stack1.items[i]);
    }

    if(FreeStack(&stack1) == STACK_E_NOT_OK){
        printf("thu hồi vùng nhớ thất bại\n");
        return -1;
    }
    printf("thu hồi vùng nhớ thành công");
    return 0;
}