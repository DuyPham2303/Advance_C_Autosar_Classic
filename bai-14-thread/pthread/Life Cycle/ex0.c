#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* task1(void* arg){
    for(int i = 0 ; i < 10 ; i++){
        printf("[%s] count %d\n",(char*)arg,i);
        sleep(1);
    }
    return NULL;
}

void* task2(void* arg){
    for(int i = 0 ; i < 10 ; i++){
        printf("[%s] count %d\n",(char*)arg,i);
        sleep(3);
    }
    return NULL;
}

int main(){
    pthread_t t1,t2;

    pthread_create(&t1,NULL,task1,"task 1");
    pthread_create(&t2,NULL,task2,"task 2");

    for(int i = 0 ; i < 5 ; i++){
        printf("[main thread] count %d\n",i);
        sleep(1);
    }

    pthread_detach(t2);
    pthread_join(t1,NULL);
    
    printf("chương trình đã kết thúc");

    return 0;
}