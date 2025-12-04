#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>

#define NUM_THREADS 5
pthread_mutex_t log_mutex; // khai báo chưa khởi tạo mutex
atomic_int cnt;
void unsafe_print(const char *s)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        putchar(s[i]); // Mỗi ký tự ghi riêng → race condition 100% xuất hiện
        usleep(100);   // Chậm một chút để bị xen kẽ rõ hơn
    }
}

void *print_log(void *arg)
{
    int id = *(int *)arg;
    int stepCnt = 0;
    while (1)
    {
        // khóa mutex để ngăn chặn các luồng khác sử dụng terminal
        //pthread_mutex_lock(&log_mutex);

        // in log ra terminal an toàn
        // printf("[Thread %d] processing cnt %d done\n", id,atomic_fetch_add(&cnt,1));
        if (id == 1)
            unsafe_print("this is thread 1\n");
        else if (id == 2)
            unsafe_print("this is thread 2\n");
        else if (id == 3)
            unsafe_print("this is thread 3\n");
        else if (id == 4)
            unsafe_print("this is thread 4\n");
        else if(id == 5) 
            unsafe_print("this is thread 5\n");
        // mở khóa mutex cho phép các luồng khác truy cập ghi log ra terminal
        //pthread_mutex_unlock(&log_mutex);
        sleep(1);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS]; // mảng lưu trữ số lượng luồng
    int ids[NUM_THREADS];           // mảng chứa id của từng luồng
    srand(time(NULL));

    // khởi tạo khóa mutex động
    pthread_mutex_init(&log_mutex, NULL);

    // Gán ID tương ứng cho từng luồng được khởi tạo
    for (int i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, print_log, &ids[i]);
    }

    while (1)
    {
        // khóa mutex để ngăn chặn các luồng khác sử dụng terminal
        //pthread_mutex_lock(&log_mutex);
        //printf("[Main thread] : processing counter %d\n", atomic_fetch_add(&cnt, 1));
        unsafe_print("this is thread main\n");
        //mở khóa mutex
        //pthread_mutex_unlock(&log_mutex);
        sleep(1);
    }

    // cơ chế joinable để đồng bộ các luồng
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Thu hồi mutex động
    pthread_mutex_destroy(&log_mutex);
    printf("All threads done.\n");
    return 0;
}
