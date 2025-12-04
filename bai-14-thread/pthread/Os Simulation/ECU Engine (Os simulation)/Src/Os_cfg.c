#include "Os_cfg.h"

extern void task_sensor();
extern void task_enginecontrol();
extern void task_actuator();
extern void task_diagnostic();

/* Bảng ánh xạ ID → entry function (do Os_Cfg.h công bố ID) */
OsTaskCtl s_task[TASK_COUNT] =
    {
        [TASK_ID_Sensor] = {.entry = task_sensor, .name = "Task Sensor"},
        /*cấu hình cho các task còn lại*/
        //...
    };

OsAlarmCtl s_alarm[ALARM_COUNT] =
    {
        [Alarm_500ms] = {.id = Alarm_500ms},
        [Alarm_1000ms] = {.id = Alarm_1000ms},
    };


