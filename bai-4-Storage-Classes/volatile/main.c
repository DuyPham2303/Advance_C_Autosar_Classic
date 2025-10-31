#include <stdio.h>
#include <stdint.h>
#include <time.h>

int *fakeReg = (int *)0x20000000;
/* 
    &fakeReg  = 0x01
    fakeReg   = 0x20000000
    *fakeReg

*/

volatile int Tempsensor;
int main()
{
    while (1)
    {
        static int count = 0;
        Tempsensor = *fakeReg;
        if (Tempsensor != 0)
        {
            printf("sensor received: %d\n", Tempsensor);
            break;
        }
        else
        {
            if (count < 5)
            {
                printf("value count: %d\n", count++);
            }
            else
            {
                *fakeReg = 23;
            }
        }
    }
    return 0;
}
