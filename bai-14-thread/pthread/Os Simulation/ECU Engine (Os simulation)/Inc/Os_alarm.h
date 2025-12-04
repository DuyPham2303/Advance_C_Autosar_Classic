#ifndef ALARM_H
#define ALARM_H

#include "Os_Types.h"
/* =================================================================================================
 * ALARM APIs (đơn vị mili-giây trong mô phỏng)
 * -------------------------------------------------------------------------------------------------
 * SetRelAlarm(alarmId, start_ms, cycle_ms)
 *  - Đặt alarm tương đối:
 *      + start_ms : trễ khởi động trước lần kích đầu tiên
 *      + cycle_ms : chu kỳ kích tiếp theo (0 = one-shot)
 *  - Ở mô phỏng: mỗi alarm là một thread ngủ theo ms → tới hạn thì ActivateTask()
 * ================================================================================================= */
StatusType SetRelAlarm(uint8_t alarmId, TickType start_ms, TickType cycle_ms);
#endif 