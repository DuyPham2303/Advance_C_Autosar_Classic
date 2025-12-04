#include "Os.h"
#include "Os_alarm.h"
#include <unistd.h>
int main(){
    if(StartOS() != E_OK){
        ShutdownOS(E_OS_STATE);
    }
    sleep(3);
    (void)SetRelAlarm(Alarm_1000ms,0u,1000u);
    sleep(30);
    ShutdownOS(E_OK);
}