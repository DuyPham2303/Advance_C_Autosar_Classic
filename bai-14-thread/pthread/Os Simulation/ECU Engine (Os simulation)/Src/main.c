#include "Os.h"
#include "Os_alarm.h"
#include <unistd.h>
int main(){
    /* 
        - gọi StartOs() -> reset cấu hình task/alarm -> kích hoạt các driven-event task tương ứng
        - gọi SetRelAlarm() -> lên lịch chạy cho periodic-event task  
        - gọi shutdownOs() -> dừng mô phỏng (nếu cần)
            -> ví dụ gọi sleep() ở main sau 1 khoảng tgian nào đó -> hệ thống tự động dừng
    */
}