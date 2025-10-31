#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;// = PTHREAD_MUTEX_INITIALIZER;
int count = 0;

void* display(void* task_name)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        printf("%s:%d\n", (char*)task_name,count++);
        pthread_mutex_unlock(&lock);
        usleep(10);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{   
    pthread_mutex_init(&lock,NULL);
    pthread_t t1; // luồng 1
    pthread_t t2; // luồng 2

    pthread_create(&t1, NULL, display,"task 1");
    pthread_create(&t2, NULL, display,"task 2");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}