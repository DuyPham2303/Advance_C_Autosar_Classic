#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <stdint.h>
// typedef struct{
//     char* name;
//     char* phone; //0..9
//     char* address; //pham van hai p3
//     uint8_t age;
// }User;
/**
 * @brief  Cấu trúc lưu thông tin của node trong linked list
 * @details 
 *  - Dùng để tạo ra dữ liệu của 1 node trước khi thêm vào linked list
 */
typedef struct Node {
    int value;          /**< Giá trị của node */
    struct Node *next;  /**< con trỏ liên kết đến node kế tiếp*/
} Node;
/**
 * @brief  Cấu trúc lưu thông tin của cây nhị phân
 * @details 
 *  - Dùng để tạo ra và quản lý các nhánh(node) trái/phải của binary tree dựa trên node giữa(gốc) 
 */
typedef struct CenterPoint {
    int value;                  /**< Giá trị của node */
    struct CenterPoint *left;   /**< con trỏ đến node con bên trái */
    struct CenterPoint *right;  /**< con trỏ đến node con bên phải*/
} CenterPoint;

/**
 * @brief               hàm thêm node vào linked list và sắp xếp từ min đến max
 * @param[in]   head    con trỏ đến con trỏ đang giữ địa chỉ node đầu của linked list
 * @param[in]   value   giá trị của node
 * @return  void
 * @retval  none
 */
void add_Node(Node **head, int value);

/**
 * @brief               hàm xây dựng cây nhị phân dựa trên linked list
 * @param[in]   head    con trỏ đến địa chỉ node đầu của linked list
 * @param[in]   start   giá trị bên trái cây    
 * @param[in]   end     giá trị bên phải cây    
 * @return  CenterPoint *
 * @retval  con trỏ đến nhánh được tạo ra trên binary tree 
 */
CenterPoint *buildTree(Node *head, int start, int end);

/**
 * @brief               hàm tìm node giữa binary tree
 * @param[in]   head    bản sao con trỏ đến địa chỉ node đầu của linked list  
 * @return  CenterPoint *
 * @retval  con trỏ đến cấu trúc binary tree lưu trữ các nhánh đã tạo ra
 */
CenterPoint *centerPoint(Node *head);

/**
 * @brief               hàm trả về nhánh tìm kiếm trên binary tree
 * @param[in]   root    bản sao con trỏ đến node gốc trên binary tree 
 * @param[in]   value   giá trị tìm kiếm của node
 * @return  CenterPoint *
 * @retval  con trỏ đến nhánh chứa value tìm được trên binary tree
 */
CenterPoint *binarySearchTree(CenterPoint *root, int value);

/**
 * @brief           hàm giải phóng các node trên linked list
 * @param[in]  head con trỏ đến con trỏ chứa địa chì đầu của linked list
 * @return  void
 * @retval  none
 */
void free_list(Node* head);

/**
 * @brief           hàm giải phóng lần lượt các nhánh(node) trên binary tree
 * @param[in]  head con trỏ đến node gốc trên binary tree
 * @return  void
 * @retval  none
 */
void free_binaryTree(CenterPoint* root);

#endif