#include <stdio.h>

int main()
{
    int *ptr = NULL;  // Gán giá trị NULL cho con trỏ 0x0000000

    if (ptr == NULL)
    {
        printf("Pointer is NULL\n");
    }
    else
    {
        printf("Pointer is not NULL\n");
    }

    int score_game = 5;
    if (ptr == NULL)
    {
        ptr = &score_game;
        *ptr = 30;
    }
    return 0;
}

