#ifndef OS_TYPES_H
#define OS_TYPES_H

#include "Std_Types.h"

typedef uint8 StatusType;
#define E_OS_OK    ((StatusType)0u)
#define E_OS_ID    ((StatusType)1u)
#define E_OS_STATE ((StatusType)2u)
#define E_OS_VALUE ((StatusType)3u)

typedef uint8 AppModeType;
#define OSDEFAULTAPPMODE ((AppModeType)0u)

typedef uint32 TickType;
typedef TickType* TickRefType;

typedef enum { COUNTER_SYSTICK = 0 } CounterType;
typedef enum { ALARM_10MS      = 0 } AlarmType;

#endif /* OS_TYPES_H */
