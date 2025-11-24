#ifndef DEBUG_H
#define DEBUG_H

#include "list.h"

/* Debug / error helper functions (implemented in debug.c) */
int list_setjmp(void);                    /* wrapper for setjmp(list_env) */
void list_log(ListStatus e, const char *msg); /* log a human-readable message */
void list_throw(ListStatus e, const char *msg); /* log then longjmp to list_env */

#endif