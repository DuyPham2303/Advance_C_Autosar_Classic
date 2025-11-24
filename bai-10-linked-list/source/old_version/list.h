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
Node *CreateNode(int val);

bool empty(Node *head);
int size(Node *head);
int get(Node *head, int pos);
void printNode(Node *head);
int front(Node *head);
int back(Node *head);
void push_back(Node **head, int value);
void push_front(Node **head, int value);
void insert(Node **head, int value, int position);
void pop_back(Node **head);
void pop_front(Node **head);
void erase(Node **head, int pos);
void clear(Node **head);

#endif