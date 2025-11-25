#include "list.h"

static Node *getLast(Node *head);

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
    if (empty(head))
    {
        printf("empty list");
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

int size(Node *head)
{
    int count = 0;
    Node *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

ListStatus front(Node *head, int *out)
{
    if (head == NULL) return LIST_EMPTY;
    if (out) *out = head->val;
    return LIST_OK;
}

ListStatus back(Node *head, int *out)
{
    if (head == NULL) return LIST_EMPTY;

    Node *last = getLast(head);
    if (out) *out = last->val;
    return LIST_OK;
}

ListStatus get(Node *head, int pos, int *out)
{
    if (head == NULL) return LIST_EMPTY;

    int listSize = size(head);
    if (pos < 0 || pos >= listSize) return LIST_OUT_OF_RANGE;

    Node *current = head;
    int index = 0;
    while (index < pos)
    {
        current = current->next;
        index++;
    }
    if (out) *out = current->val;
    return LIST_OK;
}

ListStatus push_front(Node **head, int value)
{
    if (head == NULL) return LIST_NULL;

    ListStatus status;
    Node *node = CreateNode(value, &status);
    if (status != LIST_OK) return status;

    node->next = *head;
    *head = node;
    return LIST_OK;
}

ListStatus push_back(Node **head, int value)
{
    if (head == NULL) return LIST_NULL;

    ListStatus status;
    Node *node = CreateNode(value, &status);
    if (status != LIST_OK) return status;

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
    if (head == NULL) return LIST_NULL;
    if (*head == NULL) return LIST_EMPTY;

    Node *temp = *head;
    *head = (*head)->next;
    free(temp);
    return LIST_OK;
}

ListStatus pop_back(Node **head)
{
    if (head == NULL) return LIST_NULL;
    if (*head == NULL) return LIST_EMPTY;

    Node *current = *head;

    if (current->next == NULL)
    {
        free(current);
        *head = NULL;
        return LIST_OK;
    }

    while (current->next->next != NULL)
        current = current->next;

    free(current->next);
    current->next = NULL;
    return LIST_OK;
}

ListStatus insert(Node **head, int value, int position)
{
    if (head == NULL) return LIST_NULL;

    int listSize = size(*head);
    if (position < 0 || position > listSize) return LIST_OUT_OF_RANGE;

    if (position == 0) return push_front(head, value);
    if (position == listSize) return push_back(head, value);

    ListStatus status;
    Node *new_node = CreateNode(value, &status);
    if (status != LIST_OK) return status;

    Node *current = *head;
    int index = 0;

    while (index < position - 1)
    {
        current = current->next;
        index++;
    }

    new_node->next = current->next;
    current->next = new_node;
    return LIST_OK;
}

ListStatus erase(Node **head, int position)
{
    if (head == NULL) return LIST_NULL;
    if (*head == NULL) return LIST_EMPTY;

    int listSize = size(*head);
    if (position < 0 || position >= listSize) return LIST_OUT_OF_RANGE;

    if (position == 0) return pop_front(head);
    if (position == listSize - 1) return pop_back(head);

    Node *current = *head;
    int index = 0;

    while (index < position - 1)
    {
        current = current->next;
        index++;
    }

    Node *temp = current->next;
    current->next = temp->next;
    free(temp);

    return LIST_OK;
}

ListStatus clear(Node **head)
{
    if (head == NULL) return LIST_NULL;

    Node *temp;
    while (*head != NULL)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    return LIST_OK;
}


static Node *getLast(Node *head)
{
    if (head == NULL) return NULL;
    while (head->next != NULL)
        head = head->next;
    return head;
}