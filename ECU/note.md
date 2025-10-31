# VCU -> Vehicle Control Unit 
+ Thành phần điều phối dữ liệu trong hệ thống 
+ bridge -> communication : external hardware/sensor <-> VCU <-> ECU 
    - VCU đọc status/feedback từ (pedal,phanh,drivemode,abs) -> hardware
    - gửi data (định dạng PdU) xuống các ECU phù hợp qua CAN 
    - ECU thực thi tính toán -> đưa ra lệnh thực thi (băm xung pwm chẳng hạn)
## Quy trình xử lý và thành phần của VCU

**Main Components**

`swc_VcuCmdIn`  : nhận lệnh từ VCU
    - liên tục cập nhật dữ liệu (lệnh thực thi) gửi từ VCU
    - update time : 10ms
    - Không tương tác __với Base Software layer__
    
`Swc_MotorFbAcq`: thu thập feedback từ động cơ
    - xử lý phản hồi từ cảm biến/ngoại vi bên ngoài và gọi 
    __CS__ : read sensor -> measure 
    __SR__ : publish     -> chuyển cho module xử lý
    - sample period : 10ms -> nếu có error (ko publish -> giữ sample trước đó)
`Swc_MotorCtrl` : điểu khiển + kiểm tra an toàn tối thiễu
    - kiểm tra giá trị hợp lệ __(an toàn/trong ngưỡng)__
    - tính toán tốc độ output 
    - xuất pwm cho __ActuatorCmd__ (đóng gói ở dạng Can Pdu)
    - loop period : 10ms
`Swc_ActuatorIf` : ghi tín hiệu xuống phần cứng thực tế (basic driver)
    - đọc __ActuatorCmd__ từ Can (PdU)
    - gọi __IoHwAb__ để gửi xuống xử lý
    - Ở lớp MCAL gọi ra setPWM/setDirection để ghi tín hiệu xuống các
    basic driver của MCU (Pwm,Dio)

**dataflow categorization**

`Sender/Receiver` : vận chuyển dữ liệu của hệ thống bao gồm
    - loại data __(VcuCmd)__    
        -> target speed
        -> direction
        -> stop request
    - giá trị kiểm tra/đo lường __(Meas)__
    - lệnh thực thi __(ActuatorCmd)__
`CS sync` : gọi các dịch vụ hệ thống ở lớp dưới khi cần (chuẩn đoán,xử lý lỗi)
    - __IoHwAb_Sensor/Actuator__: cung cấp các callback API để hiệu chỉnh, ổn định lại hệ thống mỗi khi có sự cố
    - Hệ thống liên tục kiểm tra __threashold__ -> quyết định khi nào gọi API từ __oHwAb_Sensor/Actuator__

-------------------------
# LUỒNG OS :triển khai từ ECUm_Init
`StartOS` : đăng ký các task, gồm 4 loại
- triển khai mapping table (taskId (biến enum) -> chuỗi tên)
- struct OsTaskCtl chứa thông tin cấu hình cho 1 task 
- sử dụng function pointer __entry__ xác định task muốn khởi tạo Os
OsAlarm : triển khai lịch xác định thời điểm các task được gọi lại
StartupHook : gọi các Init triển khai ở Base Software
    ví dụ : 
    void StartupHook(){
        Adc_Init();
    }
`ActivateTask `: chạy sau khi đăng ký các task
- dựa trên mapping table để cho phép chạy task có ID tương ứng đã đăng ký
trong module tasks -> initTask.c -> TASK() 
    -> BSW + RTE : khởi tạo 1 lần (viết testcase kiểm tra) 
    -> Alarm : Khởi tạo chu kỳ lặp hệ thống


