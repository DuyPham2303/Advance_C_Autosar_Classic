#include "Os.h"
#include "SchM.h"
#include "Rte.h"

/* Task chu kỳ 10 ms: chạy BSW main-function + runnable 10 ms của SWC */
TASK(Task_10ms)
{
    SchM_MainFunction_10ms();   /* ví dụ: Com_MainFunctionRx/Tx, IoHwAb_* */
    Rte_Run_10ms_Batch();       /* ví dụ: Swc_MotorFbAcq_Run10ms, Swc_MotorCtrl_Run10ms... */
    TerminateTask();
}
