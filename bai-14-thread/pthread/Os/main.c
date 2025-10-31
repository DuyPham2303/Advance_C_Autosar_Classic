#include "Os.h"
void *Task_Read(void *arg)
{
    while (1)
    {
        printf("[task_Read] : is handling\n");
        Os_Delay(1000);
    }
    return NULL;
}
void *Task_Calculation(void *arg)
{
    while (1)
    {
        printf("[task_Calculation] : is handling\n");
        Os_Delay(1000);
    }
    return NULL;
}
void *Task_Control(void *arg)
{
    while (1)
    {
        printf("[task_Control] : is handling\n");
        Os_Delay(1000);
    }
    return NULL;
}
void *Task_Diagnostic(void *arg)
{
    while (1)
    {
        printf("[task_Diagnostic] : is handling\n");
        Os_Delay(1000);
    }
    return NULL;
}
int main()
{
    Os_Init();
    Os_CreateTask(Task_Read, "task_Read");
    Os_CreateTask(Task_Calculation, "task_Calculation");
    Os_CreateTask(Task_Diagnostic, "task_Control");
    Os_CreateTask(Task_Control, "task_Diagnostic");

    Os_Shutdown();
}