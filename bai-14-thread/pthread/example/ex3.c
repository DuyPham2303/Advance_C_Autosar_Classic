#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdatomic.h>

atomic_int val;

// Cờ điều khiển task
bool run_flag = false;
bool exit_flag = false;
bool dataReady = false;
bool main_flag = false;
typedef enum{
    TASK_SENSOR_ID,
    TASK_CONTROL_ID,
    MAX_TASK_ID

}TASK_ID;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//pthread_cond_t sen_cond = PTHREAD_COND_INITIALIZER;
//pthread_cond_t control_cond = PTHREAD_COND_INITIALIZER;

pthread_cond_t list_cond[MAX_TASK_ID];

void Os_Init(){
    for(int i = 0 ; i < MAX_TASK_ID ; i++){
        pthread_cond_init(&list_cond[i],NULL);
    }
}
void notify_event(TASK_ID Id){
    pthread_cond_signal(&list_cond[Id]);
}
void wait_event(TASK_ID Id){
    pthread_cond_wait(&list_cond[Id],&mutex);
}
// Task cần điều khiển
void* TASK_Sensor(void* arg) {
    printf("[TASK_SENSOR] đã khởi tạo\n");
    atomic_init(&val,0);
    while (1) {
        pthread_mutex_lock(&mutex);

        // Chờ cho đến khi được phép chạy hoặc thoát
        while (!run_flag && !exit_flag) {
             wait_event(TASK_SENSOR_ID);
            //pthread_cond_wait(&sen_cond,&mutex);   //đợi điều kiện kích hoạt task sensor
        }

        if (exit_flag) {
            notify_event(TASK_CONTROL_ID);
            //pthread_cond_signal(&control_cond); //đánh thức task Control
            pthread_mutex_unlock(&mutex);
            break;
        }
        // Thực hiện công việc
        printf("[TASK_Sensor] Đang đọc...\n");

        //giả lập dữ liệu cảm biến
        int temp = rand() % 123;

        //lưu vào vùng nhớ chung
        atomic_store(&val,temp);

        dataReady = true;   //set event cho task Control
        notify_event(TASK_CONTROL_ID);
        //pthread_cond_signal(&control_cond); //đánh thức task Control

        pthread_mutex_unlock(&mutex);
        usleep(500000); // Giả lập công việc
    }

    printf("[TASK_Sensor] Kết thúc.\n");
    return NULL;
}
void* TASK_Control(void* arg){
    printf("[TASK_CONTROL] đã khởi tạo\n");
    while(1){
        pthread_mutex_lock(&mutex);
        while(!dataReady && !exit_flag){
            wait_event(TASK_CONTROL_ID);
            //pthread_cond_wait(&control_cond,&mutex);    
        }
        if(exit_flag){
            pthread_mutex_unlock(&mutex);
            main_flag = true;
            break;
        }

        //đọc dữ liệu trong vùng nhớ chung
        int temp = atomic_load(&val);
        
        printf("[TASK_Control] dữ liệu sensor: %d\n",temp);
        
        dataReady = false;      //reset event 
        pthread_mutex_unlock(&mutex);

    }
    printf("[TASK_Control] kết thúc\n");
    return NULL;
}
void* TASK_Keyboards(void* arg){
    char cmd;

    printf("Nhấn 'r' để RUN, 's' để STOP, 'q' để QUIT\n");

    printf("[TASK_Keyboard]: đã khởi tạo\n");    
    while (1) {
        scanf(" %c", &cmd);

        pthread_mutex_lock(&mutex);
        if (cmd == 'r') {
            run_flag = true;
            notify_event(TASK_SENSOR_ID);
            //pthread_cond_signal(&sen_cond); // Đánh thức task
        } else if (cmd == 's') {
            run_flag = false;
        } else if (cmd == 'q') {
            exit_flag = true;
            notify_event(TASK_SENSOR_ID);
            //pthread_cond_signal(&sen_cond); // Đánh thức nếu đang chờ
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    printf("[TASK_Keyboard]: kết thúc\n");
    return NULL;
}
// Main: điều khiển start / stop
int main() {
    srand(time(0));    
    Os_Init();
    pthread_t sensor_thread,control_thread,keyboard_thread;
    pthread_create(&keyboard_thread,NULL,TASK_Keyboards,NULL);
    pthread_create(&sensor_thread, NULL, TASK_Sensor, NULL);
    pthread_create(&control_thread,NULL,TASK_Control,NULL);


    pthread_detach(keyboard_thread);
    pthread_join(sensor_thread,NULL);
    pthread_join(control_thread, NULL);
    printf("Chương trình kết thúc.\n");
    return 0;
}
