

void Os_DelayMs(int ms) {
    usleep(ms * 1000);   // ms → microseconds
}

void* AlarmThread(void* arg)
{
    printf("[Alarm] Thread started\n");

    while (atomic_load(&alarm_active)) {
        sleep(1);  // Tick 1s
        printf("[Alarm] Tick -> SetEvent(Task_Sensor)\n");

        // Giả lập phát event cho Task Sensor
        SetEvent(&Task_Sensor, EVENT_SENSOR);
    }

    printf("[Alarm] Thread stopped\n");
    return NULL;
}




void WaitEvent(EventMaskType mask)
{
    pthread_mutex_lock(&task->mtx);
    while ((task->event_mask & mask) == 0) {  // Chưa có event mong muốn
        pthread_cond_wait(&task->cv, &task->mtx);
    }
    pthread_mutex_unlock(&task->mtx);
}





void SetEvent(TaskType *task, EventMaskType mask)
{
    pthread_mutex_lock(&task->mtx);
    task->event_mask |= mask;           // set bit event
    pthread_cond_signal(&task->cv);     // đánh thức task
    pthread_mutex_unlock(&task->mtx);
}


EventMaskType GetEvent(TaskType *task)
{
    pthread_mutex_lock(&task->mtx);
    EventMaskType current = task->event_mask;
    pthread_mutex_unlock(&task->mtx);
    return current;
}

void ClearEvent(TaskType *task, EventMaskType mask)
{
    pthread_mutex_lock(&task->mtx);
    task->event_mask &= ~mask;  // clear bit tương ứng
    pthread_mutex_unlock(&task->mtx);
}
