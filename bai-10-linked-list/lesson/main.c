#include "list.h"

int main()
{
    int len = 0,value = 0,pos = 0;
    printf("nhập số lượng node:");
    scanf("%d",&len);
    int* arr = (int*)malloc(sizeof(int)*len);
    Node *head = NULL; 
    for (int i = 0; i < len; i++){
        printf("node %d:",i);
        scanf("%d",&arr[i]);
        push_back(&head, arr[i]);
    }

    printf("PUSH BACK\n");
    printNode(head);

    printf("\nĐỌC TỪNG NODE\n");
    for(int i = 0 ; i < len ; i++)
        printf("node[%d]:%d\n",i,get(head,i));
    printf("front node: %d\tback node: %d\n",front(head),back(head));

    
    printf("\nPOP BACK\n");
    for(int i = len - 1; i >= 0 ; i--){
        pop_back(&head);
        printf("[%d]: ",i);
        printNode(head);
    }
    
    printf("\nPUSH FRONT\n");
    for (int i = 0; i < len; i++)
        push_front(&head,arr[i]);
    printNode(head);

    printf("\nPOP FRONT\n");
    for(int i = 0 ; i < len; i++){
        pop_front(&head);
        printf("[%d]: ",i);
        printNode(head);
    }

    printf("INSERT %d NODES\n",len);
    for(int i = 0 ; i < len ; i++){
        insert(&head,arr[i],i);
        printf("%d\t",arr[i]);
    }

    while(1){
        char choice;
        printf("\ninsert value:");
        scanf("%d",&value);
        printf("insert position:");
        scanf("%d",&pos);

        insert(&head,value,pos);
        printf("insert (pos:%d,val:%d)\n",pos,value);
        printNode(head);
        
        printf("erase postion:");
        scanf("%d",&pos);
        erase(&head,pos);
        
        printf("\nXÓA NODE THỨ %d\n",pos);
        printNode(head);
        
        printf("tiếp tục insert/erase ? (Y/N) ");
        scanf(" %c",&choice);
        if(choice != 'Y' && choice != 'y') break;
    }
    printf("\nXÓA TOÀN BỘ NODE\n");
    clear(&head);
    printNode(head); 

    free(arr);
    arr = NULL;

    return 0;
}