#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void cleanup(void* arg) {
    printf("Cleanup: %s\n", (char*)arg);
}

void* worker_with_exit(void* arg) {
    printf("Thread start (with pthread_exit)...\n");

    pthread_cleanup_push(cleanup, "resource A");
    pthread_cleanup_push(cleanup, "resource B");

    while(1){
        static int count = 0;
        printf("[Os] : Worker with exit\n");
        sleep(1);
        count++;
        if (count == 5) break;
    }
    printf("Thread about to exit...\n");
    pthread_exit(NULL);   // cleanup được gọi (B trước, rồi A)

    pthread_cleanup_pop(0);  // sẽ không bao giờ tới đây
    pthread_cleanup_pop(0);  
    return NULL;
}

void* worker_with_pop(void* arg) {
    printf("Thread start (with pop)...\n");

    pthread_cleanup_push(cleanup, "resource X");
    pthread_cleanup_push(cleanup, "resource Y");

    while(1){
        static int count = 0;
        printf("[Os] : Worker with pop\n");
        count++;
        sleep(1);
        if (count == 10) break;
    }

    // Thử pop(0) → không cleanup
    pthread_cleanup_pop(0);  // chỉ gỡ handler Y, không gọi
    // Thử pop(1) → gọi cleanup cho handler X
    pthread_cleanup_pop(1);

    printf("Thread return normally...\n");
    return NULL;
}

int main() {
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, worker_with_exit, NULL);
    pthread_join(t1, NULL);
    printf("=== Thread 1 finished ===\n\n");

    pthread_create(&t2, NULL, worker_with_pop, NULL);
    pthread_join(t2, NULL);
    printf("=== Thread 2 finished ===\n");

    return 0;
}
