# TỔNG QUAN VỀ HỆ THỐNG OS 
## BỐI CẢNH
+ Trong hệ thống ô tô chứa nhiều ECU xử lý nhiều chức năng khác nhau, và chúng cần phải tương tác và trao đổi tín hiệu lẫn nhau 
    để có thể đưa ra lệnh điều khiển chính xác và đúng thời điểm
    => đảm bảo ô tô hoạt động ổn định, và an toàn.
+ Lấy ví dụ đối với 1 ECU điều khiển động cơ 
    `Mục tiêu chính`: điều khiển tốc độ động cơ dựa trên set point
    `Yếu tố ngoại cảnh` : temp,voltage,current,torque 
        => cần được giám sát liên tục để thực thi chuẩn đoán 
        => điều chỉnh song song với tính toán tín hiệu điều khiển 
        => đảm bảo động cơ hoạt động ổn định, an toàn 
    `Yêu cầu thực tế` : hệ thống cần phải thực thi song song nhiều tác vụ cùng 1 lúc để điều khiển và kiểm soát tính ổn định của động cơ, 
    và điều khiển được tốc độ mong muốn
## VAI TRÒ OS 
`Ý Nghĩa`
    Các tác vụ không tự chạy mà cần Os điều phối để chúng có thể 
        - giao tiếp với nhau 
            => đúng thời điểm   : tín hiệu kích hoạt (Event,Alarm)
            => theo thứ tự      : mức ưu tiên (Priority)
        => Từ đó đưa ra các lệnh điều khiển chính xác 
`Kết luận`
    + Điều phối các Task theo sự kiện/thời gian quy định
    + quản lý việc kích hoạt/dọn dẹp tác vụ chạy trên mỗi luồng
    + đồng bộ luồng hoạt động của các tác vụ để tránh xung đột 
# QUẢN LÝ EVENT TRONG OS
`VẤN ĐỀ` 
    Nếu 1 tác vụ phụ thuộc vào tác vụ khác, cần phải xử lý như nào để chỉ kích hoạt nó đúng lúc ?
    => Giải pháp : condition variable 
`OS THỰC TẾ`
    Chứa nhiều tác vụ, mỗi tác vụ cũng cần dược quản lý bằng 
    1 cv tương ứng (Event), và được Os quản lý để các tác vụ biết
    khi nào nó sẽ được kích hoạt 
`TÓM LẠI EVENT TRONG OS THỰC TẾ ĐƯỢC QUẢN LÝ DỰA TRÊN `
        + condvar : thông báo cho các luồng phụ thuộc vào trạng thái event tương ứng
        + Biến Event : lưu trữ state các event 
        + bitmask : mặt nạ để thao tác với bit mong muốn thông qua các hành vi wait/set/get/clear
        + state   : giá trị 0/1 của bit trên EVENT MACRO EVENT
`PHÂN BIỆT CÁC LOẠI EVENT`
        + 
# QUẢN LÝ ALARM TRONG OS
## BỐI CẢNH
    Ta đã biét trong 1 Os thực tế sẽ cần phải điều phối nhiều task chạy đồng bộ với nhau, và việc kích hoạt task nào cần được phân biệt qua 
    các event tương ứng. Nếu vậy yếu tố nào sẽ quyết định
        - Khi nào các task được kích hoạt dựa trên Event
        - Đối tượng cụ thể sẽ kích hoạt chúng là ai 
=> Đó chính là `Timer Os` , cơ chế mô phỏng thời gian trong 1 Os để lên lịch (scheduling) nhằm xác định thời điểm sẽ đánh thức các task
## Mục tiêu
    - Hiểu được cách thức Os mô phỏng cơ chế Alarm thread (tick timer)
    - sử dụng Atomic_exchange() để bật/tắt Alarm an toàn giữa nhiều task thread
    - Xây dựng Alarm thread để điều phối việc trigger Event trong 1 
    Os giả lập 
## Vai trò trong ví dụ ECU 
- TASK_SENSOR sẽ được chạy trên 1 thread gọi định kỳ để cập nhật thay đổi dữ liệu cảm biến. Tuy nhiên thread này sẽ không được chạy liên tục 
mà cần được activate/deactivate đúng thời điểm dựa trên cơ chế lập lịch __(Alarm Scheduling)__ 
    => Chính vì vậy ta cần 1 thread chạy độc lập, đóng vai trò là 1 Timer để phát set Event đúng thời điểm cho các task (Thread này sẽ chạy được chạy nền, và không quan tâm đến các thread khác trong Os)
    => Khi Alarm thread trigger Event 
        -> nó sẽ sleep đến khi timer tick được update 
        -> và tiếp tục thức dậy 
        -> lặp lại chu kỳ trigger event -> sleep
## Cơ chế quản lý 
- Nếu Os shutdown đột ngột, nếu vậy làm sao dừng Alarm thread 1 cách an toàn 
    -> tránh việc thread bị kẹt mà chưa được giải phóng tài nguyên -> gây rò rỉ memory
=> Ta sẽ cần 1 flag `Atomic flag` cho phép truy cập an toàn giữa nhiều thread, để đãm bảo nếu Os shutdown thì thread hiện tại đang chạy sẽ chịu trách nhiệm dừng Alarm ngay lập tức 
## Tóm lược vai trò Alarm
- Thread nền chạy vô tận cho đến khi hệ thống dừng hoàn toàn, chịu trách nhiệm
    + sleep theo chu kỳ Tick timer (mô phỏng hardware timer)
    + gọi setEvent để trigger các task định kỳ
# QUẢN LÝ CÁU HÌNH OS
## BỐI CẢNH 
- Sau khi đã nắm được cách thức mà Os
    + Phân biệt các task được kích hoạt dựa trên Event
    + Quản lý thời điểm sẽ kích hoạt dựa trên Alarm
=> Làm sao Os biết có những Task nào ? địa chỉ ở đâu ? khởi tạo ntn ? 
    + Ta cần có thông tin cấu hình cụ thể của mỗi task bao gồm 
    => TaskType , Event , Priority 
## QUY TRÌNH KHỞI TẠO Os 
    + Quá trình Os init sẽ trải qua
        - đọc danh sách task được cấu hình
        - tạo các thread 
        - lưu thông tin điều khiển thread 
        => Yêu cầu Task configuration Table

# QUẢN LÝ GIẢI PHÓNG OS
## BỐI CẢNH
- Sau khi Os được khởi tạo và chạy đúng cách rồi thì vậy khi hệ thống dừng hoặc các task bị hủy trước khi được activate lại bởi Alarm thì ta cần phải làm như thế nào để giải phóng tài nguyên của thread đó -> chứa khối thông tin cấu hình 
=> Ta sẽ cần cơ chế để dọn dẹp tài nguyên tự động
mà không gây rò rỉ (leak) 

## VAI TRÒ CỦA CLEANUP
- Một thread giống như 1 nhân viên trong cty, khi tan ca thì phải 
    - trả lại chìa khóa phòng làm việc (mutex)
    - dọn dẹp bàn làm việc (bộ nhớ tạm - RAM)
    - Báo cáo xong mới được rời khỏi cty
=> Nếu ko dọn đúng cách (Mutex bị khóa -> RAM ko bị thu hồi -> leak memory) 

## Cơ chế đăng ký hàm dọn dẹp tự động 





