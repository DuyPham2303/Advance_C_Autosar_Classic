#ifndef CFG_H
#define CFG_H

#include "Os_Types.h"

#include <pthread.h>   /* pthread_t, mutex, condvar */
#include <stdatomic.h> /* atomic_int cho cờ “đang chạy” */

/* =====================================================================================
 * 1) TASK IDs
 * -------------------------------------------------------------------------------------
 *  Thứ tự & số lượng Task của hệ. ID dùng để gọi ActivateTask(...)
 *  và (với Extended Task) GetEvent/SetEvent/ClearEvent(...)
 * ===================================================================================== */
typedef enum
{
    TASK_ID_Sensor,
    TASK_ID_EngineControl,
    TASK_ID_Actuator,
    TASK_ID_Diagnostic,
    ISR_CAN_RX,
    TASK_COUNT
} TaskIDType;

/* GIÁ TRỊ QUY ĐỔI */
/*
    dec 	hex	    bin
    1	0x01	1 << 0
    2	0x02	1 << 1
    4	0x04	1 << 2
    8	0x08	1 << 3
    16	0x10	1 << 4
    32	0x20	1 << 5
    64	0x40	1 << 6
    128	0x80	1 << 7
*/

/* =====================================================================================
 * 2) EVENT MASKS cho driven-task
 * -------------------------------------------------------------------------------------
 *  Chỉ Task_control, task_actuator, task_diagnostic sử dụng trong mô phỏng:
 *    - EV_DataReady : dữ liệu cảm biến sẵn sàng để xử lý
 *    - EV_ControlCmdReady : tín hiệu điều khiển sẵn sàng để xử lý
 *    - EV_TriggerError    : tín hiệu lỗi cần được xử lý
 * ===================================================================================== */
#define EV_DataReady ((EventMaskType)0x0001u)
#define EV_ControlCmdReady ((EventMaskType)0x0002u)
#define EV_TriggerError ((EventMaskType)0x0004u)

/* =====================================================================================
 * 3) ALARM IDs (đơn vị mili-giây trong mô phỏng)
 * -------------------------------------------------------------------------------------
 *  Mỗi Alarm sẽ được Os_Posix.c tạo một thread “ngủ” theo start_ms/cycle_ms
 *  và tới hạn sẽ ActivateTask() Task chu kỳ tương ứng.
 * ===================================================================================== */
typedef enum
{
    Alarm_500ms, /* Kích Task_500ms mỗi 100 ms (sau start_ms) */
    Alarm_1000ms,
    ALARM_COUNT /* Số lượng Alarm (luôn để cuối cùng)        */
} AlarmIDType;

/* ****************************************************************************************
 * QUẢN LÝ TASK (Task Control)
 * **************************************************************************************** */
typedef struct OsTaskCtl
{
    pthread_t thread;     /* Thread POSIX đại diện cho Task */
    pthread_mutex_t mtx;  /* Mutex bảo vệ event mask */
    pthread_cond_t cv;    /* Condition variable để WaitEvent thức dậy */
    EventMaskType events; /* Bitmask Event đang được set */
    atomic_int running;   /* 0 = chưa chạy/đã kết thúc ; 1 = đang chạy */
    const char *name;     /* Tên để in log */
    void (*entry)();      /* Con trỏ vào hàm hàm tương ứng do tasks.c định nghĩa */
} OsTaskCtl;

/* ****************************************************************************************
 * QUẢN LÝ ALARM (mức ms)
 * **************************************************************************************** */
typedef struct OsAlarmCtl
{
    uint8_t id;        /* Alarm ID (theo Os_Cfg.h) */
    TickType start_ms; /* Trễ khởi động (ms) */
    TickType cycle_ms; /* Chu kỳ (ms); 0 = one-shot */
    pthread_t thread;  /* Thread POSIX đại diện cho Alarm */
    atomic_int active; /* 1 = đang hoạt động; 0 = dừng */
} OsAlarmCtl;

// biến toàn cục chia sẻ với Alarm.c , Os.c (bảng ánh xạ ID cho task và Alarm)
extern OsTaskCtl s_task[];
extern OsAlarmCtl s_alarm[];

#endif