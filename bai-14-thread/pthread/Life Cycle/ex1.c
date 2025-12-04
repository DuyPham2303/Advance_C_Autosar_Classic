#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// void* worker(void* arg) {
//     int* result = malloc(sizeof(int));
//     *result = 42;  // giá trị giả định
//     printf("Worker: done, result = %d\n", *result);
//     pthread_exit(result); // trả kết quả về cho join() và dừng luồng hiện tại
// }

void* worker(void* arg) {
    for(int i = 0 ; i < 5 ; i++){
        printf("worker : counts %d\n",i);
        sleep(1);
    }
    printf("worker : done\n");
    pthread_exit(NULL); //kết thúc luồng hiện tại mà không trả về kết quả
}


int main() {
    pthread_t t;

    pthread_create(&t, NULL, worker, NULL);

    /*
        join để main thread chờ thread con chạy xong
    */
    
    // void* retval;   //con trỏ đến kết quả trả về 

    // pthread_join(t, &retval);   //đồng bộ với luồng chính 
    
    // int* result = (int*)retval; //kết quả trả về sau khi được luồng con xử lý

    // printf("Main: got result = %d\n", *result); //luồng main log kết quả
    
    // free(result); //giải phóng tài nguyên của luồng con 

    
    /*
        detach khỏi main thread để thread con chạy độc lập 
    */
    pthread_detach(t);

    for(int i = 0 ; i < 15 ; i++){
        printf("[thread main] : is counting %d\n",i);
        usleep(500000);
    }
    printf("[thread main] : done");
    return 0;
}
