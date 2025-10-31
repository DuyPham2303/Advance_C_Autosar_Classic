#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Random.h"

void generateRandomElement(int *arr, int size)
{
    // Khởi tạo bộ sinh số ngẫu nhiên
    srand(time(NULL));

    // Sinh ngẫu nhiên các phần tử cho mảng
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % 20 * i; // sinh số ngẫu nhiên [0,19] * i
    }

    for (int i = 0; i < size; i++)
    {
        printf("%d\t", arr[i]);
    }

    printf("\n");
}
void generateRandomNode(Node** head,int size){
    srand(time(NULL));
    int value;
    
    for(int i = 0 ; i < size ; i++){
        value = rand() % 20 * i;
        add_Node(head,value); 
    }
    //in ra các node trong linked list

    Node* current = *head;
    while(current->next != NULL){
        current = current->next;
        printf("%d\t",current->value);
    }
    printf("\n");
}