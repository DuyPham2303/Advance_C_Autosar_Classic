#include "debug.h"
#include <stdio.h>
#include <setjmp.h>
/* Define the global jump buffer used by list_throw/list_setjmp */
jmp_buf list_env;
int list_setjmp(void)
{
    /* Returns 0 on initial call, non-zero when longjmp is invoked. */
    return setjmp(list_env);
}

void list_log(ListStatus e, const char *msg)
{
    const char *name = "UNKNOWN";
    switch (e)
    {
    case LIST_OK:
        name = "OK";
        break;
    case LIST_ALLOC_FAILED:
        name = "ALLOC_FAILED";
        break;
    case LIST_EMPTY:
        name = "EMPTY_LIST";
        break;
    case LIST_OUT_OF_RANGE:
        name = "OUT_OF_RANGE";
        break;
    case LIST_NULL:
        name = "NULL_ARG";
        break;
    default:
        break;
    }

    if (msg)
        fprintf(stderr, "[LIST][%s] %s\n", name, msg);
    else
        fprintf(stderr, "[LIST][%s]\n", name);
}

void list_throw(ListStatus e, const char *msg)
{
    /* Log the error first */
    list_log(e, msg);

    /* longjmp with a non-zero value. Use (int)e+1 to avoid zero. */
    longjmp(list_env, (int)e + 1);
}
