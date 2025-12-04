#include "Os.h"
#include "Os_cleanup.h"

#include <pthread.h>   /* pthread_t, mutex, condvar */
#include <stdatomic.h> /* atomic_int cho cờ “đang chạy” */
#include <unistd.h>    /* sleep/usleep/pause */
#include <time.h>      /* nanosleep */
#include <stdio.h>     /* printf, perror */
#include <string.h>    /* memset (nếu cần) */
#include <signal.h>    /* _exit trên POSIX */
#include <errno.h>     /* errno cho log lỗi */

/* =================================================================================================
 * 2) TASK APIs
 * -------------------------------------------------------------------------------------------------
 * ActivateTask(t)
 *  - Kích hoạt 1 task theo ID. Trong mô phỏng: mỗi lần kích tạo một pthread chạy 1 task.
 *  - Nếu task đang chạy, bản tối giản trả E_OS_LIMIT (không queue instance).
 *
 * TerminateTask()
 *  - Kết thúc task hiện tại. Trong mô phỏng: pthread_exit(NULL).
 * ================================================================================================= */


StatusType StartOS()
{
    /* Reset trạng thái các Task */
    for (int i = 0; i < TASK_COUNT; ++i)
    {
        s_task[i].thread = (pthread_t)0;
        // mặc định các bit đều được reset -> chưa có task nào được set
        s_task[i].events = 0u;
        // Khai báo trạng thái ban đầu của từng task -> mỗi task sẽ sử dụng 1 biến running để xác định trạng thái đang chạy/dừng
        (void)atomic_exchange(&s_task[i].running, 0);
        /* Mutex/cond sẽ được init lại khi ActivateTask */
    }

    /* Reset trạng thái các Alarm */
    for (int i = 0; i < ALARM_COUNT; ++i)
    {
        s_alarm[i].thread = (pthread_t)0;
        s_alarm[i].start_ms = 0u;
        s_alarm[i].cycle_ms = 0u;
        (void)atomic_exchange(&s_alarm[i].active, 0);
    }
   
    /*kích hoạt các driven-event task -> return E_OK nếu thành công*/
    //...
    

    /* khởi tạo thất bại */
    return E_OS_STATE;
}

/* Hàm trung gian để gọi cơ chế cleanup thực thi cho các Task*/
static void *OS_TaskTrampoline(void *arg)
{
    OsTaskCtl *tc = (OsTaskCtl *)arg; // con trỏ đến block lưu trữ thông tin cấu hình cho task hiện tại
    
    /*cập nhật trạng thái đã chạy cho luồng hiện tại*/
    atomic_store(&tc->running, 1);    

    /* Đăng ký cleanup: chạy khi thread return HOẶC khi pthread_exit() được gọi */
    pthread_cleanup_push(OS_TaskCleanup, tc); // đăng ký hàm dọn dẹp, sẽ chạy khi thread kết thúc bất thường.

    // con trỏ hàm để gọi ra task sẽ khởi tạo (Init)
    tc->entry();            /* Thân TASK(...) do app định nghĩa */
    pthread_cleanup_pop(1); /* 1 = thực thi cleanup ngay lúc pop */
    return NULL;
}
/* ****************************************************************************************
 * API: TASK
 * **************************************************************************************** */
StatusType ActivateTask(TaskIDType t) //
{
    // kiểm tra Task_ID có hợp lệ dựa trên enum trong Os_Cfg.h
    if (t >= TASK_COUNT)
    {
        return E_OS_ID;
    }

    OsTaskCtl *tc = &s_task[t]; // con trỏ đến thông tin cáu hình của 1 task trong bảng ánh xạ

    /* Chỉ tạo thread khi task chưa chạy */
    if (!atomic_load(&tc->running))
    {
        /* Khởi tạo sync primitives mỗi activation (đủ cho mô phỏng) */
        (void)pthread_mutex_init(&tc->mtx, NULL);
        (void)pthread_cond_init(&tc->cv, NULL);
        tc->events = 0; //reset cờ event

        const int rc = pthread_create(&tc->thread, NULL, OS_TaskTrampoline, tc);
        if (rc != 0)
        {
            perror("[OS] pthread_create (task)");
            return E_OS_STATE;
        }
        printf("[%s] is Activated\n",tc->name);
        /* Không cần join; detach để hệ thống tự thu dọn khi task kết thúc */
        (void)pthread_detach(tc->thread);
        return E_OK;
    }

    /* Task đang chạy → bản shim coi là hết slot (không queue thêm instance) */
    return E_OS_LIMIT;
}
StatusType TerminateTask(void)
{
    pthread_exit(NULL); 
    return E_OK; /* unreachable (giữ prototype) */
}

void ShutdownOS(StatusType e)
{
    (void)e;
    printf("Os has been shutdown\n");
    _exit(0); /* kết thúc tiến trình mô phỏng */
}
