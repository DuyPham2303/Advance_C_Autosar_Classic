- Giới thiệu các loại file (csv , txt)
- Trình bày các bước đọc file cơ bản
- Liên hệ ví dụ tìm kiếm thông tin user 
    + trình bày các bước : đọc file -> lưu trữ thông tin -> tạo list -> tạo binarytree -> tìm kiếm
    + triển khai ý tưởng lưu trữ các node user
- Triển khai ví dụ 
    + trình bày chức năng chính
    + Trình bày từng module theo thứ tự xử lý ở main
        -> tệp .h -> tệp .c 
- Tóm tắt nhược điểm cách triển khai truyền thống
    + vấn đề ? 
=> Phương pháp tôi ưu hơn 
- Dẫn dắt đến nội dung triển khai theo tiêu chuẩn kiến trúc phần mềm
    + Trình bày hình ảnh -> liên hệ thư viện keilC,stm32cubeide
    + Các bước triển khai quy trình phát triển phần mềm
- Ví dụ triển khai User-Management-App
    + Quy trình thu thập dữ liệu
    + Thiết kế State Diagram 
        -> luồng xử lý chính -> trạng thái hệ thống từ lúc on -> off
    + module design 
        -> Giới thiệu tổng thể các module (mối liên hệ ? chức năng ? đặc điểm phân chia layer ?)
        -> Triển khai từng layer từ low -> hight
            + userData -> UserNode -> CenterPoint (.h)
            + FileOperation -> NodeUser -> BuildTree (.h) và SharedData
            + Menu,keyboard (.h)
            + UserApp (.h)
        -> Trình bày theo quy trình state diagram
            + Init system -> UserMenu -> handler search,show list,endprogram
=> chạy chương trình 
    

