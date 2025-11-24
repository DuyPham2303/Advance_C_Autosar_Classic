#include "add.h"
#include "sub.h"
#include "test1.h"
#include "test2.h"
#include <stdio.h>

int main(void)
{
    printf("this is main\n");
    printf("add = %d\nsub = %d\n",add(10,3),sub(21,2));
    test1();
    test2();
    return 0;
}
