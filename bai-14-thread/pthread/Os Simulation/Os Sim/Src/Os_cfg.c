#include "Os_cfg.h"

extern void task_sensor();
extern void task_enginecontrol();
extern void task_actuator();
extern void task_diagnostic();

/* Bảng ánh xạ ID → entry function (do Os_Cfg.h công bố ID) */
OsTaskCtl s_task[TASK_COUNT] =
    {
        [TASK_ID_Sensor] = {.entry = task_sensor, .name = "Task Sensor"},
        [TASK_ID_EngineControl] = {.entry = task_enginecontrol, .name = "Task EngineControl"},
        [TASK_ID_Actuator] = {.entry = task_actuator, .name = "Task Actuator"},
        [TASK_ID_Diagnostic] = {.entry = task_diagnostic, .name = "Task Diagnostic"},
    };

OsAlarmCtl s_alarm[ALARM_COUNT] =
    {
        [Alarm_500ms] = {.id = Alarm_500ms},
        [Alarm_1000ms] = {.id = Alarm_1000ms},
    };


