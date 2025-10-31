#include "list.h"

Node *CreateNode(int val)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->next = NULL;
    node->val = val;
    return node;
}
bool empty(Node *head)
{
    return (head == NULL);
}
void printNode(Node *head)
{
    if (empty(head))            
    {                           
        printf("empty list\n"); 
        return;                 
    }
    Node *current = head;
    while (current != NULL)
    {
        printf("%d\t", current->val);
        current = current->next;
    }
    printf("\n");
}
void push_back(Node **head, int value)
{
    // tạo node mới
    Node *node = CreateNode(value);

    // kiểm tra và cập nhật node đầu nếu danh sách rỗng
    if (empty(*head))
    {
        *head = node;
        return;
    }

    // gán địa chỉ node đầu
    Node *current = *head;

    // duyệt cho đến khi gặp node cuối
    while (current->next != NULL)
    {
        current = current->next;
    }

    // cập nhật node cuối
    current->next = node;
}
void pop_back(Node **head)
{
    // nếu danh sách rỗng thì dừng xử lý
    if (empty(*head))            
    {                           
        printf("empty list\n"); 
        return;                 
    }
    Node *current = *head;

    // xử lý nếu chỉ có 1 node
    if (current->next == NULL)
    {
        free(current); // giải phóng node đầu do current đang trỏ tới
        *head = NULL;  // cập nhật con trỏ đầu list
        return;
    }

    // xử lý nếu có 2 node trở lên
    while (current->next->next != NULL)
    {
        current = current->next;
    }

    Node *last_node = current->next; // lưu node cuối
    free(last_node);                 // giải phóng node cuối cũ
    current->next = NULL;            // cập nhật node cuối mới
}
int size(Node *head)
{
    if (empty(head))
        return 0;
    else
    {
        Node *current = head;
        int size = 1;
        while (current->next != NULL)
        {
            current = current->next;
            size++;
        }
        return size;
    }
}
int front(Node *head)
{
    if (empty(head))            
    {                           
        printf("empty list\n"); 
        return 0;                 
    }
    return head->val;
}
int back(Node *head)
{
    if (empty(head))            
    {                           
        printf("empty list\n"); 
        return 0;                 
    }
    Node *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    return current->val;
}
int get(Node *head, int pos)
{

    if (empty(head))            
    {                           
        printf("empty list\n"); 
        return 0;                 
    }

    // trả về node đầu nếu pos là giá trị âm
    if (pos <= 0) return front(head);

    // trả về node cuối nếu pos > vị trí cuối list
    else if (pos >=  size(head) - 1) return back(head);

    // trả về node tương ứng với giá trị pos
    else
    {
        Node *current = head;
        int index = 0;

        // duyệt đến khi gặp node ở vị trí pos tương ứng
        while (index < pos)
        {
            current = current->next;
            index++;
        }
        return current->val;
    }
}
void push_front(Node **head, int value)
{
    // tạo node mới
    Node *node = CreateNode(value);

    // cập nhật node đầu nếu danh sách rỗng
    if (empty(*head))
    {
        *head = node;
        return;
    }
    // cập nhật node đầu của list
    node->next = *head;
    *head = node;
}
void pop_front(Node **head)
{
    if (empty(*head))            
    {                           
        printf("empty list\n"); 
        return;                 
    }

    Node *current = *head;
   
    //trường hợp chỉ có 1 node
    if (current->next == NULL)
    {
        free(current);  //xóa node duy nhất của list
        *head = NULL;   //cập nhật con trỏ gốc của list
        return;
    }
    //trường hợp có 2 node trở lên
    *head = (*head)->next; //cập nhật node đầu mới của list trước khi xóa 
    free(current);         //xóa node đã lưu
}
void insert(Node **head, int value, int position)
{
    // xủ lý chèn ở đầu list
    if (position <= 0)            push_front(head, value);
    // xử lý chèn ở cuối list
    else if (position >= size(*head)) push_back(head, value);
    // xủ lý chèn ở các vị trí còn lại
    else
    {
        Node *new_node = CreateNode(value);

        Node *current = *head;
        int index = 0;

        // duyệt đến vị trí trước node cần xóa
        while (index < position - 1)
        {
            current = current->next;
            index++;
        }
        // cập nhật node mới tại vị trí insert
        new_node->next = current->next;
        current->next = new_node;
    }
}

void erase(Node **head, int position)
{
    if (empty(*head))            
    {                           
        printf("empty list\n"); 
        return;                 
    }

    //xóa node đầu nếu vị trí <= 0
    if (position <= 0)                      pop_front(head);
    
    //xóa node cuối nếu vị trí >= số node lưu trữ trong list
    else if (position >= size(*head) - 1)   pop_back(head);
    else
    {
        Node *current = *head;
        int index = 0;

        //duyệt đến vị trí trước node cần xóa
        while (index < position - 1)
        {
            current = current->next;
            index++;
        }

        // lưu trữ node cần xóa
        Node *temp = current->next;

        // cập nhật node hiện tại trỏ đến địa chỉ sau node cần xóa
        current->next = current->next->next;

        // xóa node đã lưu
        free(temp);
    }
}
void clear(Node **head)
{
    if (empty(*head))            
    {                           
        printf("empty list\n"); 
        return;                 
    }
    Node *temp = NULL;
    while (*head != NULL)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    *head = NULL;
}