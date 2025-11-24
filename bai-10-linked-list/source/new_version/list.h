#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node 
{
    struct Node *next; 
    int val;
}Node;

/* Error codes for list operations. Using legacy enum names as requested. */
typedef enum ListStatus
{
    LIST_OK = 0,
    LIST_EMPTY,          /* Operation on empty list */
    LIST_OUT_OF_RANGE,   /* Index out of range */
    LIST_ALLOC_FAILED,   /* Out of memory / allocation failure */
    LIST_NULL            /* Invalid/null argument */
}ListStatus;

Node *CreateNode(int val, ListStatus *status);

// API khởi tạo node
Node *CreateNode(int val, ListStatus *status);

// In toàn bộ list
void printNode(Node *head);

// Lấy kích thước list
int size(Node *head);

// Lấy giá trị node đầu tiên
ListStatus front(Node *head, int *out);

// Lấy giá trị node cuối cùng
ListStatus back(Node *head, int *out);

// Lấy giá trị tại vị trí bất kỳ
ListStatus get(Node *head, int pos, int *out);

// Thêm node vào đầu list
ListStatus push_front(Node **head, int value);

// Thêm node vào cuối list
ListStatus push_back(Node **head, int value);

// Xoá node đầu tiên
ListStatus pop_front(Node **head);

// Xoá node cuối cùng
ListStatus pop_back(Node **head);

// Chèn node ở vị trí bất kỳ
ListStatus insert(Node **head, int value, int position);

// Xoá node ở vị trí bất kỳ
ListStatus erase(Node **head, int position);

// Xoá toàn bộ list
ListStatus clear(Node **head);

#endif // LIST_H
