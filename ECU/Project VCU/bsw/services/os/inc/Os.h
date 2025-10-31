#ifndef OS_H
#define OS_H

#include "Os_Types.h"
#include "Os_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

StatusType StartOS(AppModeType mode);
void       ShutdownOS(StatusType error);

StatusType GetCounterValue(CounterType counter, TickRefType value);
StatusType IncrementCounter(CounterType counter);
StatusType SetRelAlarm(AlarmType alarm, TickType offset, TickType cycle);
StatusType CancelAlarm(AlarmType alarm);

StatusType Os_ConnectAlarm(AlarmType alarm, void (*cb)(void));
StatusType Os_DisconnectAlarm(AlarmType alarm);

void DisableAllInterrupts(void);
void EnableAllInterrupts(void);

void StartupHook(void);
void ShutdownHook(StatusType error);
void ErrorHook(StatusType error);

void Os_TickISR(void);

#ifdef __cplusplus
}
#endif
#endif /* OS_H */
