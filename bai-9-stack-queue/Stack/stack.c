#include "stack.h"

static bool Stack_IsEmpty(const Stack* stack)
{
    return (stack == NULL || stack->top == -1);
}

static bool Stack_IsFull(const Stack* stack)
{
    if (stack == NULL) return true;
    return (stack->top == stack->size - 1);
}

StackStatus Stack_Init(Stack* stack, int size)
{
    if (stack == NULL || size <= 0)
        return STACK_INVALID_ARG;

    stack->items = (int*)malloc(sizeof(int) * size);
    if (stack->items == NULL)
        return STACK_MEMORY_ERROR;

    stack->size = size;
    stack->top = -1;

    return STACK_INIT_OK;
}

StackStatus Stack_Push(Stack* stack, int value)
{
    if (stack == NULL)
        return STACK_INVALID_ARG;

    if (Stack_IsFull(stack))
        return STACK_FULL;

    stack->items[++stack->top] = value;
    return STACK_HANDLE_OK;
}

StackStatus Stack_Pop(Stack* stack, int* outValue)
{
    if (stack == NULL || outValue == NULL)
        return STACK_INVALID_ARG;

    if (Stack_IsEmpty(stack))
        return STACK_EMPTY;

    *outValue = stack->items[stack->top--];
    return STACK_HANDLE_OK;
}

StackStatus Stack_Top(const Stack* stack, int* outValue)
{
    if (stack == NULL || outValue == NULL)
        return STACK_INVALID_ARG;

    if (Stack_IsEmpty(stack))
        return STACK_EMPTY;

    *outValue = stack->items[stack->top];
    return STACK_HANDLE_OK;
}

StackStatus Stack_Free(Stack* stack)
{
    if (stack == NULL)
        return STACK_INVALID_ARG;

    if (stack->items != NULL)
    {
        free(stack->items);
        stack->items = NULL;
    }

    stack->size = 0;
    stack->top = -1;

    return STACK_FREE_OK;
}
