#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//khởi tạo khóa mutex và condition variable
pthread_mutex_t key = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//dữ liệu cảm biến chia sẻ giữa nhiều luồng
int sensor_data = 0; 

//lưu trạng thái dữ liệu cảm biến 
bool dataReceived = false;

void* task_sensor(void* arg) {
    while (1) {
        sleep(1); // Giả lập thời gian đọc cảm biến

        pthread_mutex_lock(&key);
        sensor_data = rand() % 100;  // Giả lập dữ liệu cảm biến
        dataReceived = true;              // cập nhật trạng thái đã đọc cảm biến
        printf("[Sensor Read] Dữ liệu cảm biến: %d\n", sensor_data);
        pthread_cond_signal(&cond);  // Báo cho thread xử lý biết có dữ liệu mới
        pthread_mutex_unlock(&key);
    }
    return NULL;
}

void* task_calculate(void* arg) {
    while (1) {
        pthread_mutex_lock(&key);
        while (!dataReceived) {
            pthread_cond_wait(&cond, &key);  // Chờ đến khi có dữ liệu
        }

        //lưu dữ liệu cảm biến vào biến tạm để xử lý
        int data = sensor_data;

        dataReceived = false;   //reset trạng thái để chờ đọc dữ liệu mới
        pthread_mutex_unlock(&key);

        //giả lập xử lý tín hiệu cảm biến
        int result = data * data; 
        printf("[Processing data Sensor] Dữ liệu đã xử lý: %d\n", result);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t t1, t2;

    pthread_create(&t1, NULL, task_sensor, NULL);
    pthread_create(&t2, NULL, task_calculate, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
