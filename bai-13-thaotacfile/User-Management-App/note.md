# NỘI DUNG 1 : THAO TÁC FILE VÀ VÍ DỤ
- Giới thiệu các loại file (csv , txt)
- Trình bày các bước đọc file cơ bản
- Liên hệ ví dụ tìm kiếm thông tin user 
    + trình bày các bước : đọc file -> lưu trữ thông tin -> tạo list -> tạo binarytree -> tìm kiếm
    + triển khai ý tưởng lưu trữ các node user
- Triển khai ví dụ 
    + trình bày chức năng chính
    + Trình bày từng module theo thứ tự xử lý ở main
        -> tệp .h -> tệp .c 
# NÔI DUNG 2 : THIẾT KẾ PHẦN MỀM THEO TIÊU CHUẨN
**Dẫn dắt**
- Tóm tắt nhược điểm cách triển khai ví dụ ở nội dung 1
    + Đặt vấn đề : 
    ví dụ trên bắt đầu triển khai phức tạp -> nhiều công đoạn xử lý -> dễ gây lỗi dây chuyển 
        -> phân tích ví dụ đẻ thấy các lỗi 
            + phần đọc/ghi dữ liệu triển khai cùng lúc với phần giao diện hiển thị
                => khó quản lý và đọc hiễu 
            + chia sẽ API toàn cục trong toàn project
            + chia sẻ dữ liệu không an toàn giữa các module
    => Không phù hợp cho 1 dự án thực tế
**Trình bày Slide**
- __Nội dung lý thuyết__
    - Kiến trúc phần mềm ?
        + Cách thức tổ chức 1 hệ thống thành các lớp và module, nhằm mô hình hóa và khái quát các bước triển khai chi tiết, giúp Coder
            - Xác định được lộ trình các bước sẽ làm theo thứ tự
            - quản lý chương trình 1 cách hiệu quả
                -> dễ xử lý lỗi (module độc lập)
                -> mở rộng thêm nhiều tính năng (ko cần sữa toàn bộ code)
            - dễ dàng phối hợp teamwork
    - Tại sao cần ?
        - dự án lớn phức tạp -> nhiều coder join -> cần tiêu chuẩn chung để thống nhất 
            + cách thức tổ chức hệ thống -> ai responsible module nào ?
            + dễ dàng cho bất kỳ ai hiểu được
    - So sánh với cách truyền thống ?
- __Thực hành__
    - Các bước triển khai : kết hợp slide + biểu đồ
    - Demo Project mẫu
        `Bước 1: khai báo function prototype`
            + Triển khai lần lượt các module (LOW -> HIGH)
            + Hàm ? cấu trúc ? 
            + mối quan hệ giữa các module 
        `Bước 2: Triển khai định nghĩa các module (LOW -> HIGH)`
            + Viết các hàm trong .c dựa trên flowchart 
            + Triển khai lần lượt từng tính năng 
                - Service layer call Structure layer   
                    -> triển khai Abstraction API __(chi tiết xử lý tính năng)__
                - Application layer call Service layer 
                    -> triển khai Abstraction API __(giao diện hiển thị tương tác người dùng)__
                - Main program call Application layer
                    -> Thiết kế luồng xử lý hệ thống  __(Mô tả vòng đời của hệ thống)__


---------------------------------

Bước 1: Xây dựng cấu trúc project
    -> tạo ra folder và module tương ứng -> .c và .h
Bước 2: triển khai định nghĩa cho từng .h (LOW -> HIGH)
    -> Định nghĩa ra struct,union,enum 
    -> comment (mô tả chức năng của file)

    

