#include <stdio.h>

int v = 0;
int r = 0;

int main()
{
    for (int i = 0; i < 1000000; i++)
    {
        if (i == 50)
        {
            v++;
            r++;
        }
        else if(i > 80){
            v++;
            r++;
        }
    }
    printf("v = %d, r = %d\n", v, r);
    return 0;
}
