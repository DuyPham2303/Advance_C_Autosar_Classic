#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
/* 
    MÔ HÌNH Producer - Consumer
    + Producer : bên tạo dữ liệu
    + Consumer : bên nhận dữ liệu
    => cả 2 cùng chia sẻ vùng nhớ chung, được đồng bộ dựa trên mutex - condvar
*/


//biến mutex điều phối các luồng thực thi an toàn
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

//biến điều kiện để quản lý cơ chế wait/notify
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 

//flag chung để luồng wait/signal truy cập để thông báo/chờ tín hiệu
int ready = 0;

void* worker(void* arg) {
    pthread_mutex_lock(&mtx);

    if(!ready){
        pthread_cond_wait(&cond, &mtx); //cơ chế đưa luồng vào trạng thái ngủ tạm thời
    }
    printf("Worker: được đánh thức!\n");
    /* Xử lý nhiệm vụ cụ thể sau khi được signaler thông báo*/
    pthread_mutex_unlock(&mtx);
}

void* signaler(void* arg) {
    /* xử lý nhiệm vụ cụ thể trước khi notify cho worker */
    sleep(1);   //mô phỏng thời gian chờ xử lý 
    pthread_mutex_lock(&mtx);
    ready = 1;  //cập nhật flag
    
    pthread_cond_signal(&cond);     //thông báo cho 1 luồng duy nhất
    pthread_cond_broadcast(&cond);  //thông báo nhiều luồng
    
    pthread_mutex_unlock(&mtx);
}
