# TRÌNH BÀY BỐI CẢNH
    - nhắc lại mảng 
        + địa chỉ sắp xếp ? -> cách truy cập ?
        + xóa ở vị trí bất kỳ ?
            - gán 0 hoặc ghi ký tự null '\0'
            - dịch << address 
            - adjust memory (realloc)
        + thêm ở vị trí bất kỳ
            - adjust memory (realloc)
            - dịch >> address 
            - ghi data vào address trống
        => waste time -> slow processing
   **Note** : Triển khai chương trình lớn cần quan tâm tốc độ xử lý 
            => ảnh hưởng hiệu suất hệ thống -> Linkest list tối ưu so với array
## LÝ THUYẾT
    - So sánh với Array (giống ? khác ?)
    - Triển khai cấu trúc 
        + node ? 
        + truy cập đến node ? (con trỏ ?) => (pointer + i) ? => chỉ áp dụng với elements có linear address
        => sử dụng con trỏ next (trỏ tới bất kỳ pointer nào)
    - Outline ra cấu trúc hình ảnh
   **Note** : 1 chuỗi các node
    => chứa 2 đặc điểm : data và next pointer
## TRIỂN KHAI 
    - Tóm tắt lại cấu trúc linkest list trên slide 
   **Triển khai code**
    __define struct__:
        + lưu ý về khai báo member struct của chính struct đó
    __Hàm init__:
        + sự khác biệt init 2 cách : `return void` và `return pointer` -> triển khai cả 2 
        + tạo từng node riêng biệt
        + liên kết từng node thành danh sách
    __Hàm hiển thị__
        + demo theo các thứ tự link khác nhau 
    __So sánh với queue và stack__
        + quy định nguyên tắc hoạt động FIFO , LIFO 
            -> chỉ cho phép thêm/xóa ở vị trí quy định
            -> kiểm tra full/empty
            -> truy cập bằng cách lặp/duyệt qua các node (địa chỉ node ko liển kề)
        + linkest list linh hoạt 
            -> thêm/xóa bất kể vị trí
            -> truy cập bằng chỉ số 
            -> kiểm tra empty
    __Các hàm thay đổi__
        + pushback
            -> truyền double pointer ?
            -> dùng biến tạm để duyệt qua node thay vì trực tiếp trên node gốc ?
        + giải thích hướng triển khai các hàm còn lai
----------------------------------------------------
# VẤN ĐỀ
1) Hàm insert 
    + gọi size quá nhiều -> gây
        - rối cho user đọc code (ko hiễu size là gì)
        - giảm hiệu suất (bên trong size chứa logic xử lý while)
    + chưa có điều kiện ngăn chặn việc nhập số âm
        - thông báo "giá trị ko hợp lệ" còn mơ hồ
2) Ứng dụng 3 loại
    + linked list
        - thao tác nhanh,linh hoạt -> chèn xóa bất kể vị trí
        - quản lý phức tạp -> thông qua con trỏ
        - ứng dụng quản lý dữ liệu nhiều đối tượng
            -> cập nhật liên tục 
            -> xóa bất kỳ node nào ko cần thiết -> nhanh chóng,linh hoạt -> tối ưu memory
    + queue
        - thao tác thêm đuôi/xóa đầu -> FIFO
        - xử lý chuỗi dữ liệu truyền giữa 2 MCU -> cần đảm bảo truyền và nhận theo thứ tự
    Stack
        - thao tác thêm/xóa đầu -> LIFO
        - Xứ lý lưu trữ trạng thái  (màn hình,trang web)


    
        



    
            
    