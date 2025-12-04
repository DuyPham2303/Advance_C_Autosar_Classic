#include "list.h"

static Node *getLast(Node *head);

void list_log(ListStatus st, const char *msg)
{

    const char *name = "unknown";

    switch (st)
    {
    case LIST_OK:
        name = "OK";
        break;
    case LIST_EMPTY:
        name = "EMPTY";
        break;
    case LIST_OUT_OF_RANGE:
        name = "OUT OF RANGE";
        break;
    case LIST_ALLOC_FAILED:
        name = "ALLOC FAILED";
        break;
    case LIST_NULL:
        name = "NULL ARGS";
        break;
    default:
        break;
    }

    if(msg)
        printf("[%s] : %s\n",name,msg);
    else
        printf("[%s] : unknown msg\n",name);
}
// Tạo node, kiểm tra lỗi cấp phát
Node *CreateNode(int val, ListStatus *status)
{
    *status = LIST_OK;
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
    {
        *status = LIST_ALLOC_FAILED;
        return NULL;
    }
    node->val = val;
    node->next = NULL;
    return node;
}

void printNode(Node *head)
{
    if (head == NULL)
    {
        printf("empty list");
        return;
    }
    Node *currentt = head;
    printf("print nodes : ");
    while (currentt != NULL)
    {
        printf("%d\t", currentt->val);
        currentt = currentt->next;
    }
    printf("\n");
}

int size(Node *head)
{
    int count = 0;
    Node *currentt = head;
    while (currentt != NULL)
    {
        count++;
        currentt = currentt->next;
    }
    return count;
}

ListStatus front(Node *head, int *out)
{
    if (head == NULL)
        return LIST_EMPTY;
    if (out)
        *out = head->val;
    return LIST_OK;
}

ListStatus back(Node *head, int *out)
{
    if (head == NULL)
        return LIST_EMPTY;

    Node *last = getLast(head);
    if (out)
        *out = last->val;
    return LIST_OK;
}

ListStatus push_front(Node **head, int value)
{
    if (head == NULL)
        return LIST_NULL;

    ListStatus status;
    Node *node = CreateNode(value, &status);
    if (status != LIST_OK)
        return status;

    node->next = *head;
    *head = node;
    return LIST_OK;
}

ListStatus push_back(Node **head, int value)
{
    if (head == NULL)
        return LIST_NULL;

    ListStatus status;
    Node *node = CreateNode(value, &status);
    if (status != LIST_OK)
        return status;

    if (*head == NULL)
    {
        *head = node;
        return LIST_OK;
    }

    Node *last = getLast(*head);
    last->next = node;
    return LIST_OK;
}

ListStatus pop_front(Node **head)
{
    if (head == NULL)
        return LIST_NULL;
    if (*head == NULL)
        return LIST_EMPTY;

    Node *temp = *head;
    *head = (*head)->next;
    free(temp);
    return LIST_OK;
}

ListStatus pop_back(Node **head)
{
    if (head == NULL)
        return LIST_NULL;
    if (*head == NULL)
        return LIST_EMPTY;

    Node *currentt = *head;

    if (currentt->next == NULL)
    {
        free(currentt);
        *head = NULL;
        return LIST_OK;
    }

    while (currentt->next->next != NULL)
        currentt = currentt->next;

    free(currentt->next);
    currentt->next = NULL;
    return LIST_OK;
}

// Chèn node ở vị trí bất kỳ
ListStatus insert(Node **head, int value, int position)
{
    if (head == NULL)
        return LIST_NULL;

    int listsize = size(*head);

    // kiểm tra vị trí hợp lệ -> giá trị position có nằm trong khoảng cho phép
    // if (position < 1 || position > listsize + 1)
    //     return LIST_OUT_OF_RANGE;

    // Xử lý thêm ở đầu / cuối list
    if (position <= 1)
        return push_front(head, value);

    else if (position >= listsize + 1)
        return push_back(head, value);

    else
    {
        ListStatus st;

        Node *new_node = CreateNode(value, &st);

        if (st != LIST_OK)
            return st;

        int index = 1;

        Node *currentt = *head;

        // lặp đến node ở trước vị trí cần insert
        while (index < position - 1)
        {
            /* cach 1 */
            currentt = currentt->next;
            /* cach 2 */
            //(*head) = (*head)->next;
            index++;
        }

        // cập nhật vị trí của node cần insert
        new_node->next = currentt->next;
        currentt->next = new_node;
        return st;
    }
}

// Xoá node ở vị trí bất kỳ
ListStatus erase(Node **head, int position)
{
    if (head == NULL)
        return LIST_NULL;

    if (*head == NULL)
        return LIST_EMPTY;

    int listsize = size(*head);

    // kiểm tra position có hợp lệ
    // if (position < 1 || position > listsize)
    //     return LIST_OUT_OF_RANGE;

    // Xử lý xóa ở đầu / cuối list
    if (position <= 1)
        return pop_front(head);

    else if (position >= listsize)
        return pop_back(head);

    Node *currentt = *head;
    int index = 1;

    while (index < position - 1)
    {
        currentt = currentt->next;
        index++;
    }

    Node *temp = currentt->next;
    currentt->next = temp->next;
    free(temp);

    return LIST_OK;
}

// Xoá toàn bộ list
ListStatus clear(Node **head)
{
    if (head == NULL)
        return LIST_NULL;

    Node *temp;

    while (*head != NULL)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    return LIST_OK;
}

ListStatus get(Node *head, int pos, int *out)
{
    if (head == NULL)
        return LIST_EMPTY;

    int listsize = size(head);

    // xử lý đọc node đầu / cuối
    if (pos == 1)
        return front(head, out);

    else if (pos > listsize)
        return back(head, out);

    Node *current = head;
    int index = 1;

    while (index < pos)
    {
        current = current->next;
        index++;
    }

    *out = current->val;
    return LIST_OK;
}

static Node *getLast(Node *head)
{
    if (head == NULL)
        return NULL;
    while (head->next != NULL)
        head = head->next;
    return head;
}