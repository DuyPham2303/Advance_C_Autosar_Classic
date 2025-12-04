#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void* SensorTask(void* arg) {
    float* sensor_value = (float*)malloc(sizeof(float));
    // Giả lập đọc cảm biến ngẫu nhiên
    *sensor_value = 20.0f + (rand() % 1000) / 100.0f; // 20.00–30.00
    pthread_exit(sensor_value); // Trả về giá trị đo được
}

int main() {
    pthread_t tid;
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {  // mô phỏng 100 chu kỳ (≈1s)
        void* ret_val;
        pthread_create(&tid, NULL, SensorTask, NULL);

        //note : dùng detach ko lấy kết quả trẻ về được
        pthread_join(tid, &ret_val); // đợi thread con kết thúc

        float value = *(float*)ret_val;
        free(ret_val);
        // In liên tục trên cùng 1 dòng (không xuống dòng)
        printf("\r[ECU] Sensor value: %.2f °C", value);
        fflush(stdout); // buộc in ra ngay

        usleep(10 * 1000); // 10 ms
    }

    printf("\nDone.\n");
    return 0;
}
