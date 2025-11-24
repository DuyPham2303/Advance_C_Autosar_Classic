#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <setjmp.h>

typedef struct Node 
{
    struct Node *next; 
    int val;
}Node;
Node *CreateNode(int val);
/* Error codes for list operations. Add new values here if needed. */
typedef enum ListError
{
    LIST_OK = 0,
    LIST_ERR_OOM,          /* Out of memory */
    LIST_ERR_EMPTY,        /* Operation on empty list */
    LIST_ERR_OUT_OF_RANGE, /* Index out of range */
    LIST_ERR_INVALID_ARG   /* Invalid argument */
} ListError;

/* Backwards-compatible alias: some modules use the older `ListStatus` name.
   Provide an alias so both identifiers are available regardless of which
   header is included. */
typedef ListError ListStatus;

/* Global jmp buffer used by the simple list error handling helpers.
   Call `list_setjmp()` (or `setjmp(list_env)` directly) in your high-level
   caller to establish a recovery point before invoking list operations that
   may `list_throw()` on error. Example:

   if (list_setjmp() == 0) {
       // normal path: call list functions
   } else {
       // error recovery path: a ListError code was thrown
   }
*/
extern jmp_buf list_env;

/* Debug / error helper functions (implemented in debug.c) */
int list_setjmp(void);                    /* wrapper for setjmp(list_env) */
void list_log(ListError e, const char *msg); /* log a human-readable message */
void list_throw(ListError e, const char *msg); /* log then longjmp to list_env */

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