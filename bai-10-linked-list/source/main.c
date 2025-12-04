#include "list.h"
#include <stdio.h>
void Menu(){
    printf("\n-----------------TEST MENU-----------------\n");
    printf("0. Exit\n");
    printf("1. Push Front\n");
    printf("2. Push back\n");
    printf("3. Pop Front\n");
    printf("4. Pop back\n");
    printf("5. insert\n");
    printf("6. erase\n");
    printf("7. Front\n");
    printf("8. back\n");
    printf("9. get\n");
    printf("10. print nodes\n");
    printf("11. clear");
    printf("\n--------------------------------------------\n");
}

int main(void)
{
    Node* head = NULL;
    int running = 1;

    while(running){
        Menu();
        int option;
        printf("nhap option: ");
        if(scanf("%d",&option) != 1){
            int c;
            while((c = getchar()) != '\n' && c != EOF){}
            goto ErrorlogInput;
        }
        switch(option){
            case 0:
            {
                running = 0;
                break;
            }
            case 1:
            {
                int out;
                printf("value : ");
                if(scanf("%d",&out) != 1){
                    goto ErrorlogInput;
                }
                ListStatus st = push_front(&head,out);
                if(st != LIST_OK)
                    list_log(st,"push front failed");
                break;
            }
            case 2:
            {
                int out;
                printf("value : ");
                if(scanf("%d",&out) != 1){
                    goto ErrorlogInput;
                }
                ListStatus st = push_back(&head,out);
                if(st != LIST_OK)
                    list_log(st,"push back failed");
                break;
            }
            case 3:
            {
                ListStatus st = pop_front(&head);
                if(st != LIST_OK)
                    list_log(st,"pop front failed");
                break;
            }
            case 4:
            {
                ListStatus st = pop_back(&head);
                if(st != LIST_OK)
                    list_log(st,"pop back failed");
                break;
            }
            case 5:
            {
                int pos , val;
                printf("insert position: ");
                if(scanf("%d",&pos) != 1) goto ErrorlogInput;

                printf("insert value: ");
                if(scanf("%d",&val) != 1) goto ErrorlogInput;

                ListStatus st = insert(&head,val,pos);
                if(st != LIST_OK)
                    list_log(st,"insert failed");
                break;
            }
            case 6:
            {
                int pos;
            
                printf("erase position: ");
                if(scanf("%d",&pos) != 1) goto ErrorlogInput;

                ListStatus st = erase(&head,pos);
                if(st != LIST_OK)
                    list_log(st,"erase failed");
                break;
            }
            case 7:
            {
                int out;
                ListStatus st = front(head,&out);
                if(st == LIST_OK) printf("front = %d\n",out);
                else list_log(st,"front failed");
                break;
            }
            case 8:
            {
                int out;
                ListStatus st = back(head,&out);
                if(st == LIST_OK) printf("back = %d\n",out);
                else list_log(st,"back failed");
                break;
            }

            case 9:
            {
                int pos;
                printf("get position: ");
                if(scanf("%d",&pos) != 1) goto ErrorlogInput;
                
                int out;
                ListStatus st = get(head,pos,&out);
                if(st == LIST_OK) printf("get position : %d",out);
                else list_log(st,"get position failed");
                break;
            }

            case 10:
            {
                printNode(head);
                break;
            }
            case 11:
            {
                ListStatus st = clear(&head);
                if(st != LIST_OK)
                    list_log(st,"clear failed");
                break;
            }
            ErrorlogInput:
                printf("invalid input");
                break;
        }
    }
    
    if(head)
    {
        ListStatus st = clear(&head);
        if(st != LIST_OK){
            list_log(st,"clear failed");
            return 1;
        }
    }
    printf("chuong trinh ket thuc");
    return 0;
}