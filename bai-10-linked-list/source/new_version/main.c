#include "list.h"
#include "debug.h"
#include <stdio.h>

void partial_Test()
{
    int code = list_setjmp();
    if (code == 0)
    {
        printf("Entering protected block\n");

        Node *head = NULL;
        ListStatus st = push_back(&head, 10);
        if (st != LIST_OK)
        {
            list_log(st, "push_back failed");
        }
        else
        {
            int v;
            if (front(head, &v) == LIST_OK)
                printf("front = %d\n", v);
        }

        // Simulate an error that should be caught by the recovery point
        list_throw(LIST_ALLOC_FAILED, "simulated allocation failure");

        // This line will not be reached
        printf("This will not print\n");
    }
    else
    {
        ListStatus err = (ListStatus)(code - 1);
        printf("Recovered from error (code=%d)\n", err);
    }
}
void Full_Test()
{
    /* Establish a recovery point for longjmp-based error handling. */
    int jmp_ret = list_setjmp();
    if (jmp_ret != 0)
    {
        ListStatus err = (ListStatus)(jmp_ret - 1);
        printf("Recovered from thrown error: %d\n", err);
    }

    Node *head = NULL;
    int running = 1;

    while (running)
    {
        printf("\n-- Linked List Test Menu --\n");
        printf("1) push_front <value>\n");
        printf("2) push_back <value>\n");
        printf("3) pop_front\n");
        printf("4) pop_back\n");
        printf("5) insert <pos> <value>\n");
        printf("6) erase <pos>\n");
        printf("7) front\n");
        printf("8) back\n");
        printf("9) get <pos>\n");
        printf("10) size\n");
        printf("11) print\n");
        printf("12) clear\n");
        printf("0) exit\n");
        printf("Choose an option: ");

        int opt = -1;
        if (scanf("%d", &opt) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            printf("Invalid selection\n");
            continue;
        }

        switch (opt)
        {
        case 0:
            running = 0;
            break;

        case 1:
        {
            int v;
            printf("value: ");
            if (scanf("%d", &v) != 1)
            {
                printf("bad value\n");
                break;
            }
            ListStatus st = push_front(&head, v);
            if (st != LIST_OK)
            {
                list_log(st, "push_front failed");
                if (st == LIST_ALLOC_FAILED)
                    list_throw(st, "push_front allocation failed");
            }
            break;
        }

        case 2:
        {
            int v;
            printf("value: ");
            if (scanf("%d", &v) != 1)
            {
                printf("bad value\n");
                break;
            }
            ListStatus st = push_back(&head, v);
            if (st != LIST_OK)
            {
                list_log(st, "push_back failed");
                if (st == LIST_ALLOC_FAILED)
                    list_throw(st, "push_back allocation failed");
            }
            break;
        }

        case 3:
        {
            ListStatus st = pop_front(&head);
            if (st != LIST_OK)
                list_log(st, "pop_front failed");
            break;
        }

        case 4:
        {
            ListStatus st = pop_back(&head);
            if (st != LIST_OK)
                list_log(st, "pop_back failed");
            break;
        }

        case 5:
        {
            int pos, val;
            printf("pos value: ");
            if (scanf("%d %d", &pos, &val) != 2)
            {
                printf("bad input\n");
                break;
            }
            ListStatus st = insert(&head, val, pos);
            if (st != LIST_OK)
                list_log(st, "insert failed");
            break;
        }

        case 6:
        {
            int pos;
            printf("pos: ");
            if (scanf("%d", &pos) != 1)
            {
                printf("bad input\n");
                break;
            }
            ListStatus st = erase(&head, pos);
            if (st != LIST_OK)
                list_log(st, "erase failed");
            break;
        }

        case 7:
        {
            int out;
            ListStatus st = front(head, &out);
            if (st == LIST_OK)
                printf("front = %d\n", out);
            else
                list_log(st, "front failed");
            break;
        }

        case 8:
        {
            int out;
            ListStatus st = back(head, &out);
            if (st == LIST_OK)
                printf("back = %d\n", out);
            else
                list_log(st, "back failed");
            break;
        }

        case 9:
        {
            int pos;
            printf("pos: ");
            if (scanf("%d", &pos) != 1)
            {
                printf("bad input\n");
                break;
            }
            int out;
            ListStatus st = get(head, pos, &out);
            if (st == LIST_OK)
                printf("get(%d) = %d\n", pos, out);
            else
                list_log(st, "get failed");
            break;
        }

        case 10:
            printf("size = %d\n", size(head));
            break;

        case 11:
            printNode(head);
            break;

        case 12:
        {
            ListStatus st = clear(&head);
            if (st != LIST_OK)
                list_log(st, "clear failed");
            break;
        }

        default:
            printf("Unknown option\n");
            break;
        }

        /* clear trailing input line */
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
    }

    /* Final cleanup */
    if (head)
        clear(&head);
    printf("Exiting\n");
}
int main(void)
{
    ListStatus st;
    Node *head = CreateNode(12, &st);
    Node *node1 = CreateNode(10, &st);
    Node *node2 = CreateNode(5, &st);

    // 12 -> 10 -> 5
    head->next = node1;
    node1->next = node2;

    printNode(head);
    printf("%d", size(head));
    return 0;
}