#include <stdio.h>
#include <stdlib.h>
#include "BubbleSort.h"
#include "BinaryTree.h"
#include "BinarySearch.h"
#include "Random.h"

#define SIZE 200

int main()
{
    /*  
     int find;
     int arr[SIZE];
     generateRandomElement(arr, SIZE);

     printf("sau khi sắp xếp\n");
     bubble_sort(arr, SIZE);

     for(int i = 0 ; i < SIZE ; i++){
         printf("%d\t",arr[i]);
     }

     while(1){
         printf("\nNhập giá trị cần tìm: ");
         scanf("%d", &find);

         int result = BinarySearch(arr, 0, SIZE - 1, find);
         if (result == -1)
         {
             printf("Không tìm thấy %d trong mảng.\n", find);
         }
         else
         {
             printf("Tìm thấy %d tại vị trí %d trong mảng.\n", find, result);
         }
         char promt;
         printf("continue (Y/N) ?");
         scanf(" %c",&promt);
         if(promt != 'y' && promt != 'Y')
             break;
     } */

    int find;
    Node *head = NULL;

    generateRandomNode(&head, SIZE); // tạo node với value ngẫu nhiên

    while (1)
    {
        printf("Nhập giá trị cần tìm: ");
        scanf("%d",&find);

        CenterPoint *root = centerPoint(head); // xây dựng cây binary dựa trên linked list đã sắp xếp

        
        CenterPoint *result = binarySearchTree(root, find); // kết quả tìm kiếm

        if (!result)
        {
            printf("Không tìm thấy %d trong binary tree.\n", find);
        }
        else
        {
            printf("Tìm thấy %d trong binary tree.\n", result->value);
        }
        char promt;
        printf("continue (Y/N) ?");
        scanf(" %c", &promt);
        if (promt != 'y' && promt != 'Y')
        {
            free_binaryTree(root);
            free_list(head);
            break;
        }
    }
    return 0;
}
