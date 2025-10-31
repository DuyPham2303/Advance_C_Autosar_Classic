#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "binaryTree.h"

void add_Node(Node **head, int value) {

    /* 
        {12,3,44,10,29,30}

        head -> 3 -> 10 -> 12 -> 29 -> 30 -> 44 
    */
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = NULL;

    //thêm vào đầu linked list nếu empty hoặc giá trị node mới > node đầu
    if (*head == NULL || (*head)->value >= value){
        new_node->next = *head;
        *head = new_node;   
        return;
    }

    
    Node *current = *head;
    //kiểm tra cho đến khi gặp node cuối và giá trị node tiếp theo trong list lớn hơn node mới tạo
    while (current->next != NULL && (*current->next).value < value) {
        current = current->next;
    }
    //chèn node mới 
    new_node->next = current->next;
    current->next = new_node; 
}

CenterPoint *buildTree(Node *head, int start, int end) {
    //nếu không có node nào hoặc danh sách không hợp lệ -> dừng xử lý
    if (head == NULL || start > end) {
        return NULL;
    }
    //xác định điểm giữa list
    int mid = (start + end) / 2;
    Node *node = head;
    //lặp cho đến khi gặp node giữa
    for (int i = start; i < mid; i++) {
        if (node->next == NULL) {
            break;
        }
        node = node->next;
    }

    //cấp phát bộ nhớ cho node giữa 
    CenterPoint *root = (CenterPoint *) malloc(sizeof(CenterPoint));
    root->value = node->value;
    //tạo ra các node con từ node giữa (các nhánh của cây nhị phân)
    root->left = buildTree(head, start, mid - 1); 
    root->right = buildTree(node->next, mid + 1, end);

    return root;    //trả về node của nhánh hiện tại
}


CenterPoint *centerPoint(Node *head) {
    int length = 0;
    Node *node = head;
    
    //duyệt qua list và đếm số lượng node
    while (node != NULL) {
        node = node->next;
        length++;
    }

    //trả về cấu trúc binary tree chứa các nhánh đã tạo ra
    return buildTree(head, 0, length - 1);
}
Node* Linear_Search(Node* head,int search){
    int loop = 1;
    Node* current = head;
    while(current->next != NULL){
        current = current->next;   
        printf("so lan lap: %d\n", loop++);
        if(current->value == search) return current;
    }
    return NULL;
}
//ham tim kiem nhi phan
CenterPoint *binarySearchTree(CenterPoint *root, int search) {
    static int loop = 0;
    loop++; 
    printf("so lan lap: %d\n", loop);
    if (root == NULL) {
        loop = 0;
        return NULL;
    }

    if (root->value == search) {
        loop = 0;
        return root;
    }

    //trường hợp tìm kếm
    if (root->value > search) {
        //tìm bên trái binary tree
        return binarySearchTree(root->left, search);
    } else {
        //tìm bên phải binary tree
        return binarySearchTree(root->right, search);
    }
}

void free_list(Node *head)
{
    Node *temp = NULL;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
void free_binaryTree(CenterPoint* root) {
    if (root == NULL) return;
    free_binaryTree(root->left);
    free_binaryTree(root->right);
    free(root);
}

