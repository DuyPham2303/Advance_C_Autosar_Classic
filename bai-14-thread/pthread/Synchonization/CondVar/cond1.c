#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;
int data = 0;

void* task_calculation(void* arg) {
    char* nameTask = (char*)arg;
    while(1){
        pthread_mutex_lock(&mtx);
        if(!ready){
            printf("[%s] : is sleeping\n",nameTask);
            pthread_cond_wait(&cond, &mtx);
        }
        printf("[%s] : reading data %d\n",nameTask,data);
        ready = 0;
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

void* task_sensor(void* arg) {
    char* nameTask = (char*)arg;
    
    while(1){
        pthread_mutex_lock(&mtx);
        data = rand() % 50; //mô phỏng giá trị ngẫu nhiên [0...49]

        //chỉ signal khi thỏa điều kiện
        if(data > 20){
            printf("[%s] : notify task_calculation\n",nameTask);
            ready = 1;
            pthread_cond_signal(&cond);
        }
        else{
            printf("[%s] : processing\n",nameTask);
        }
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
    return NULL;
}

#define MAX_task_calculation 3

int main(){
    pthread_t t1,t2;
    srand(time(NULL));

    pthread_create(&t1,NULL,task_calculation,"task_calculation");
    pthread_create(&t2,NULL,task_sensor,"task_sensor");

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    return 0;
}
