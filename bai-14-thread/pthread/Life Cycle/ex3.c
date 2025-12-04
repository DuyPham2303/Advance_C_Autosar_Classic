#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Thread nhỏ: gửi lỗi CAN rồi thoát
void* ErrorReporter(void* arg) {
    int errorCode = *((int*)arg);
    printf("[ErrorReporter] Gửi mã lỗi %d lên CAN bus...\n", errorCode);
    sleep(1); // giả lập thời gian gửi
    printf("[ErrorReporter] Hoàn tất, thoát.\n");
    free(arg);  // giải phóng bộ nhớ
    return NULL;
}

// Thread chính ECU: điều khiển động cơ
void* EngineControlTask(void* arg) {
    for (int cycle = 1; cycle <= 25; cycle++) {
        printf("[EngineControl] Chu kỳ %d: đọc cảm biến, tính toán...\n", cycle);
        sleep(1);

        // giả lập phát hiện lỗi ở các chu kỳ khác nhau
        if (cycle == 3 || cycle == 9 || cycle == 15 || cycle == 20) { 
            int* code = malloc(sizeof(int));
            *code = 42; // giả lập mã lỗi
            pthread_t tid;
            pthread_create(&tid, NULL, ErrorReporter, code);
            pthread_detach(tid); // Không cần join, thread tự dọn sau khi xong
            printf("[EngineControl] Đã tạo ErrorReporter để báo lỗi.\n");
        }
    }
    return NULL;
}

int main() {
    pthread_t engine;
    pthread_create(&engine, NULL, EngineControlTask, NULL);

    pthread_join(engine, NULL); // đợi engine task kết thúc
    printf("ECU tắt.\n");
    return 0;
}
