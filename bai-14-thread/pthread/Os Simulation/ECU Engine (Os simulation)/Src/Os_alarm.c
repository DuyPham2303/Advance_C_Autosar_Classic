#include "Os_alarm.h"
#include "Os_cfg.h"

#include <pthread.h>   /* pthread_t, mutex, condvar */
#include <stdatomic.h> /* atomic_int cho cờ “đang chạy” */
#include <unistd.h>    /* sleep/usleep/pause */
#include <time.h>      /* nanosleep */
#include <stdio.h>     /* printf, perror */
#include <string.h>    /* memset (nếu cần) */
#include <signal.h>    /* _exit trên POSIX */
#include <errno.h>     /* errno cho log lỗi */

// định nghĩa ở Os.c
extern StatusType ActivateTask(TaskIDType t);
extern StatusType TerminateTask(void);

/* ****************************************************************************************
 * TIỆN ÍCH THỜI GIAN
 * **************************************************************************************** */
static void OS_SleepMs(uint32_t ms)
{
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)((ms % 1000u) * 1000000L);
    (void)nanosleep(&ts, NULL);
}

static void *OS_AlarmThread(void *arg)
{
    OsAlarmCtl *const a = (OsAlarmCtl *)arg; // con trỏ đến block chứa cấu hình báo thức cho task chạy theo chu kỳ
    atomic_store(&a->active, 1);             // set trạng thái bắt đầu chạy
    /* Trễ khởi động một lần (nếu có) */
    if (a->start_ms > 0u)
    {
        OS_SleepMs(a->start_ms); // nạp thời gian bắt đầu xử lý vào bộ đếm
    }

    /* Vòng kích hoạt chu kỳ */
    while (atomic_load(&a->active)) // kiểm tra trạng thái của báo thức liên tục
    {
        //printf("[Alarm thread] triggered\n");
        printf("---------------------------------\n");
        {
            /* Ánh xạ alarm → task chu kỳ tương ứng */
            switch (a->id)
            {
            case Alarm_500ms:
                // kích hoạt periodic task tương ứng
                break;
            case Alarm_1000ms:
                // kích hoạt periodic task tương ứng
                break;
            default:
                printf("Không hỗ trợ Alarm hiện tại\n");
                break;
            }

            /* One-shot: thoát vòng nếu cycle_ms == 0 */
            if (a->cycle_ms == 0u)
            {
                break;
            }
            OS_SleepMs(a->cycle_ms); // thời gian lặp
        }
    }
    atomic_store(&a->active, 0); // reset trạng thái báo thức
    return NULL;
}

StatusType SetRelAlarm(uint8_t alarmId, TickType start_ms, TickType cycle_ms)
{
    // kiểm tra Alarm_ID có hợp lệ dựa trên enum trong Os_Cfg.h
    if (alarmId >= ALARM_COUNT)
    {
        return E_OS_ID;
    }

    // con trỏ tới thông tin cấu hình alarm trong bảng ánh xạ loại báo thức cho task chạy theo chu kỳ
    OsAlarmCtl *const a = &s_alarm[alarmId];

    // cấu hình thời gian lặp lại báo thức cho task (chỉ gán 1 lần)
    a->start_ms = start_ms;
    a->cycle_ms = cycle_ms;

    // khởi tạo luồng chạy cho task chạy theo báo thức
    const int rc = pthread_create(&a->thread, NULL, OS_AlarmThread, a);
    if (rc != 0)
    {
        perror("[OS] pthread_create (alarm)");
        return E_OS_STATE;
    }

    /* Alarm cũng detach để tự thu dọn */
    (void)pthread_detach(a->thread); // Luồng Alarm chạy trên nền -> ko cần join để báo kết quả về hàm khởi tạo nó
    return E_OK;
}