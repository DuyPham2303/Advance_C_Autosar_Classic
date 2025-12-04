#include "Os_event.h"

StatusType WaitEvent(TaskIDType id,EventMaskType m)
{
    OsTaskCtl *const tc = &s_task[id];

    (void)pthread_mutex_lock(&tc->mtx);
    while ((tc->events & m) == 0u)
    {
        (void)pthread_cond_wait(&tc->cv, &tc->mtx);
    }
    (void)pthread_mutex_unlock(&tc->mtx);
    return E_OK;
}

StatusType SetEvent(TaskIDType id, EventMaskType m)
{
    if (id >= TASK_COUNT)
    {
        return E_OS_ID;
    }

    OsTaskCtl *const tc = &s_task[id];

    (void)pthread_mutex_lock(&tc->mtx);
    tc->events |= m;                       /* set bit */
    (void)pthread_cond_broadcast(&tc->cv); /* đánh thức task đang WaitEvent */
    (void)pthread_mutex_unlock(&tc->mtx);

    return E_OK;
}

StatusType GetEvent(TaskIDType id, EventMaskType *m)
{
    if ((id >= TASK_COUNT) || (m == NULL))
    {
        return E_OS_ID;
    }

    OsTaskCtl *const tc = &s_task[id];

    (void)pthread_mutex_lock(&tc->mtx);
    *m = tc->events;
    (void)pthread_mutex_unlock(&tc->mtx);
    return E_OK;
}

StatusType ClearEvent(TaskIDType id,EventMaskType m)
{
    /* Giả định gọi từ Task_Com trong mô phỏng */
    OsTaskCtl *const tc = &s_task[id];

    (void)pthread_mutex_lock(&tc->mtx);
    tc->events &= (EventMaskType)(~m);
    (void)pthread_mutex_unlock(&tc->mtx);
    return E_OK;
}
