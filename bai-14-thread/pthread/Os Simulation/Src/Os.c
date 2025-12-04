TaskConfigType Os_TaskList[] = {
    { "Task_Com",    Task_Com },
    { "Task_Diag",   Task_Diag },
    { "Task_Sensor", Task_Sensor },
};

void StartOS(void)
{
    int num_tasks = sizeof(Os_TaskList) / sizeof(TaskConfigType);
    for (int i = 0; i < num_tasks; ++i) {
        pthread_create(&Os_TaskList[i].handle, NULL, Os_TaskList[i].entry, NULL);
        printf("[OS] Created %s\n", Os_TaskList[i].name);
    }
}
