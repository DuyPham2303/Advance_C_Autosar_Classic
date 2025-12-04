#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>
#define ALARM_ms     100     //thời gian báo thức (ms)
#define DELAY_ms     1000   
#define ALARM_COUNTER   30  //số lần ALARM lặp lại

//biến toàn cục để mô phỏng việc đọc/ghi dữ liệu giữa nhiều luồng
atomic_int updated_sensor = 0;
atomic_int running = 1;

//cấu trúc chứa thông tin cấu hình của task Os
typedef struct OsTaskCtrl{
    pthread_mutex_t mx;
    pthread_t   tid;
    char* name;
}OsTaskCtrl;

//bảng cấu hình thông số cho task duy nhất
OsTaskCtrl Os_ConfigTable = {
    .name = "task_sensor",
    .tid = (pthread_t)0
};

//giải phóng worker tự động
void cleanup_worker(void* arg){
    OsTaskCtrl* const s = (OsTaskCtrl*)arg;
    printf("%s done reclaim resources\n",s->name);
    pthread_mutex_destroy(&s->mx);
    s->tid = (pthread_t)0;
}

// Thread worker: làm 1 việc rồi dừng
void* Task_sensor(void* arg) {
    OsTaskCtrl* const s = (OsTaskCtrl*)arg;
    srand(time(0));
    //đăng ký hàm cleanup sẽ được gọi khi luồng thực hiện xong
    pthread_cleanup_push(cleanup_worker,s);

    //truy cập data và xử lý dữ liệu
    pthread_mutex_lock(&s->mx);

    printf("[%s]đang đọc...\n", s->name);
    int temp = rand() % 100;    //mô phỏng đọc data ngẫu nhiên
    atomic_store(&updated_sensor,temp);    //ghi dự liệu vào biến toàn cục an toàn
    printf("[%s]Hoàn thành, thoát!\n",s->name);
    
    //bỏ truy cập data, cho phép các luồng khác sử dụng
    pthread_mutex_unlock(&s->mx);

    pthread_cleanup_pop(1); //gọi hàm cleanup để thu hồi tài nguyên 

    return NULL;
}
// Thread scheduler: tạo worker theo lịch
void* scheduler_thread(void* arg) {
    static int currentCycle = 0;
    while (currentCycle < ALARM_COUNTER) {    //chu kỳ lặp bào thức 
        static int AlarmTick = 0;
        if(AlarmTick < ALARM_ms){
            AlarmTick++; //mô phỏng biến đếm tràn trong timer 
        }
        else{
            pthread_mutex_init(&Os_ConfigTable.mx,NULL);
            //khởi tạo 
            if (pthread_create(&Os_ConfigTable.tid, NULL, Task_sensor,&Os_ConfigTable) != 0) {
                perror("pthread_create worker failed");
                continue;
            }

            pthread_detach(Os_ConfigTable.tid);   

            printf("[Scheduler] Đã tạo %s\n",Os_ConfigTable.name);
            AlarmTick = 0;   //reset biến đếm tràn timer
            currentCycle++; //cập nhật số lần lặp Alarm 
        }
        usleep(DELAY_ms);
    }
    printf("[Os Scheduler] : Terminater\n");
    atomic_store(&running,0);
    return NULL;
}

int main() {
    pthread_t sched;
    pthread_mutex_t main_mx;
    pthread_mutex_init(&main_mx,NULL);

    if (pthread_create(&sched, NULL, scheduler_thread, NULL) != 0) {
        perror("pthread_create scheduler failed");
        return 1;
    }

    pthread_detach(sched);

    //mô phỏng luồng chính cập nhật dữ liệu từ task_sensor
    while(atomic_load(&running)){
        pthread_mutex_lock(&main_mx);
        printf("[main thread] dữ liệu cập nhật từ %s : %d\n",Os_ConfigTable.name,atomic_load(&updated_sensor));
        pthread_mutex_unlock(&main_mx);
        sleep(1);
    }

    pthread_mutex_destroy(&main_mx);
    printf("[Main Os] : Terminated\n");

    return 0;
}
