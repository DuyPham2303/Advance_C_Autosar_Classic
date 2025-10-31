+ Giới thiệu memory layout
    => Khác biệt chương trình lưu trữ trên máy tính và MCU
        -> nơi lưu trữ chương trình
        -> file thực thi đầu ra
    => gồm 5 phân vùng 
    => compiler trên macOS và window khác gì nhau
# TEXT
    - lưu mã lệnh chương trình (trên MCU là flash), chuỗi hằng số
    - chỉ đọc/ko cho phép ghi
# BSS
    - lưu biến global,static bằng 0/chưa khởi tạo giá trị, const (chỉ dọc)
    - đọc và ghi
    - vùng nhớ tồn tại đến khi chương trình kết thúc
# DATA
    - lưu biến global,static đã khởi tạo giá trị const (chỉ dọc)
    - đọc và ghi
    - vùng nhớ tồn tại đến khi chương trình kết thúc
# STACK
    - tham số hàm, biến local, biến const (thay đổi được qua con trỏ)
    - tồn tại từ khi được gọi đến khi ra khỏi phạm vi định nghĩa
    - quản lý theo cơ chế LIFO
    - Cấp phát nhanh, tự động bởi chương trình tại thời điểm biên dịch
    - cấp phát vượt quá size -> stackOverflow 
# HEAP
    - cấp phát tại thời điểm runtime 
    - quản lý bởi user thông qua các từ khóa
    - dùng con trỏ để quản lý vùng nhớ được cấp phát
    - cấp phát chậm so với stack
    - kích thước lớn hơn stack
    - lỗi memory leak nếu không giải phóng memory sau khi sử dụng
        -> vùng nhớ ko còn sử dụng nhưng chưa được thu hồi 
    - Lỗi dangling pointer nếu ko gán NULL sau khi free memory
        -> con trỏ có nguy cơ trỏ tới vùng nhớ ko xác định -> nếu thực hiện truy xuất giá trị  
# HEAP VS STACK
------------------------
**Lưu ý**
## a) static (global + local) : 
    -> không càn trình bày rõ trên assembly 
## b) string literal đều được lưu ở .rdata 
    -> con trỏ,mảng có thể lưu ở .data/.rdata/.stack
    -> mảng lưu trữ ký tự (bản sao của sring literal sẽ được sinh ra và lưu ở .data -> thay đổi được nội dung)
## c) sửa lại dấu mũi tên đi lên của heap ko phải của stack
## d) lý do realloc lại nhưng địa chỉ ban đầu bị thay đổi
    -> địa chỉ liền kề sau vùng nhớ cũ đã được sử dụng nên Os cần phải cấp phát vùng địa chỉ mới có các giá trị địa chỉ liền kề tương ứng với số byte được cấp phát để lưu trữ kích thước mới đã được điều chỉnh
