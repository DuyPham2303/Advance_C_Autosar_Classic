#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int count = 0;
void* task1(void* var)
{
    //static int count = 0;
    while(1)
    {
        printf("count task1: %d\n", count++);
        //sleep(1);
        usleep(10);
    }
}

void* task2(void* var)
{
    //static int count = 0;
    while(1)
    {
        printf("count task2: %d\n", count++);
        //sleep(2);
        usleep(20);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1,NULL,task1,NULL);
    pthread_create(&t2,NULL,task2,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    return 0;
}