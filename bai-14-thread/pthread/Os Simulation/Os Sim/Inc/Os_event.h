#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include "Os_Cfg.h"

/* =================================================================================================
 * -------------------------------------------------------------------------------------------------
 * WaitEvent(mask)
 *  - Task (ví dụ Task_Com) chờ cho tới khi bất kỳ bit trong “mask” được set.
 *  - Mô phỏng dùng mutex+condvar. Chỉ dùng cho Task_Com trong mẫu này.
 *
 * SetEvent(t, mask)
 *  - Set các bit event cho task “t” và đánh thức task đang WaitEvent().
 *
 * GetEvent(t, *mask)
 *  - Đọc mặt nạ event hiện tại của task “t”.
 *
 * ClearEvent(mask)
 *  - Xoá (clear) các bit event đã xử lý. Gọi từ task đang chạy.
* ================================================================================================= */

StatusType WaitEvent(TaskIDType id,EventMaskType m);
StatusType SetEvent(TaskIDType t, EventMaskType m);
StatusType GetEvent(TaskIDType t, EventMaskType* m);
StatusType ClearEvent(TaskIDType id,EventMaskType m);
#endif 