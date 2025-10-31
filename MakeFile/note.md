# Dẫn dắt
    - khi build bình thường bằng lệnh gcc -> tốn tgian,dễ lỗi
# Giới thiệu
- tệp văn bản, chứa các chỉ thị cho phép build program tự động,nhanh chóng
- chứa các biến,quy tắc,câu lệnh 
- chạy bằng tool make
- Cho phép biên dịch,liên kết tự động nhiều phần của 1 project  
    -> hạn chế lỗi do biên dịch thủ công
    -> tự động đọc file và chỉ biên dịch các phần thay đổi trong chương trình => tăng tốc độ biên dịch
# Tạo makefile
    - 3 cách tạo file
        + Makefile
        + makefile
        + file.mk
# Thành phần makefile
    - rules (quy tắc) ? -> chứa lệnh muốn thực thi
    ví dụ : target: dependency
                commands
    => ý nghĩa các phần trên ?
    ví dụ : chạy compiler 
        -> triển khai các rule (phase 1 -> 4, run:)
        -> triển khai rule xóa file