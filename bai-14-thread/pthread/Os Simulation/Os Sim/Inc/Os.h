#ifndef OS_H
#define OS_H
#include "Os_cfg.h"
/* =================================================================================================
 * 1) LIFECYCLE
 * -------------------------------------------------------------------------------------------------
 * StartOS(appMode)
 *  - Khởi động OS, reset trạng thái, gọi StartupHook(), autostart InitTask.
 *  - Bản mô phỏng không trả về (giữ process sống); dùng ShutdownOS để thoát.
 *
 * ShutdownOS(error)
 *  - Gọi ShutdownHook(error) và kết thúc tiến trình mô phỏng.
 * ================================================================================================= */
StatusType StartOS();
void       ShutdownOS(StatusType error);

/* =================================================================================================
 * 2) TASK APIs
 * -------------------------------------------------------------------------------------------------
 * ActivateTask(t)
 *  - Kích hoạt 1 task theo ID. Trong mô phỏng: mỗi lần kích tạo một pthread chạy TASK().
 *  - Nếu task đang chạy, bản tối giản trả E_OS_LIMIT (không queue instance).
 *
 * TerminateTask()
 *  - Kết thúc task hiện tại. Trong mô phỏng: pthread_exit(NULL).
 * ================================================================================================= */
StatusType ActivateTask(TaskIDType t);
StatusType TerminateTask(void);

void task_sensor();
void task_enginecontrol();
void task_actuator();
void task_diagnostic();

#endif 