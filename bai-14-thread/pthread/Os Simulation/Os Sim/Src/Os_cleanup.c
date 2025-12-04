#include "Os_cleanup.h"
#include "Os_cfg.h"
#include <stdio.h>

extern StatusType TerminateTask();
void OS_TaskCleanup(void *arg)
{
    OsTaskCtl *const tc = (OsTaskCtl *)arg;
    pthread_cond_destroy(&tc->cv);
    pthread_mutex_destroy(&tc->mtx);
    atomic_store(&tc->running, 0); /* đánh dấu task đã kết thúc (cho phép kích lại) */
    tc->thread = (pthread_t)NULL;  /* reset địa chỉ của task Os cần chạy lại */
    printf("[CleanUp %s] : is executed\n",tc->name);
    TerminateTask();
}