# DẪN DẮT BỐI CẢNH
    - Ứng dụng IoT ? -> cách thức truyền data ? có giống MCU -> MCU ? -> decode binary
    => mã hóa thành dạng phù hợp để truyền 
# LÝ THUYẾT JSON
   **Là gì ? áp dụng ? phạm vi ?**
        + most common (java script)
   **So sánh stack,queue,linked list**
        + stack, queue -> dựa trên array
        + linked list -> node liên kết vs nhau
        + json -> cách lưu trữ dữ liệu , dịnh dạng văn bản (chưa có cách hoạt động cụ thể )
   **key - value ?**
    - định dạng ? các loại ? (`object` và `array`)
    => Ví dụ về file json trong .vscode
        + mục đích ? ai xử lý khi chạy ?
# TRIỂN KHAI 
   ## PHÂN TÍCH JSON
        - Cách thức hoạt động ? xử lý dữ liệu ?
        - Chuyển từ dạng json sang dạng chuỗi " " -> phân tích lấy ra data 
        - ví dụ 1 chuỗi json đơn giản + struct đơn giản -> ko tối ưu ? -> thêm device -> cập nhật struct -> waste time
        => triển khai struct TỔNG QUÁT
   ## CÁC BƯỚC 
        - danh sách enum -> xác định loại data trong chuỗi json 
        - struct jsonvalue -> cấu trúc định nghĩa key - value
            -> triển khai ví dụ đơn giản để minh họa việc phân tách
        - Ví dụ minh họa code trong main 
   ## PHÂN TÍCH CHUỖI THỰC TẾ
        - triển khai chuỗi trên nhiều dòng -> 2 chuỗi viết liền kề -> tự gộp thành 1 chuỗi
        - ký tự '\' có ý nghĩa gì ?
        - giói thiệu các hàm (mục đích ? đặc điểm ? các bước ?)
        - Ví dụ phân tách 1 chuỗi json -> debug để giải thích 
        - convert string -> số nguyên 
    
"{
    Ten : DuyPham,
    GPA : 7.5.
    IsStudent : true
}"

JSON_TYPE:  JSON_OBJECT
    + key
        - JSON_TYPE : JSON_STRING 
        - string    : Ten
    + value
        - JSON_TYPE : JSON_STRING
        - string    : 7.5
    
    + key
        - JSON_TYPE : JSON_STRING 
        - string    : GPA
    + value
        - JSON_TYPE : JSON_NUMBER
        - number    : 7.5

    + key
        - JSON_TYPE : JSON_STRING 
        - string    : GPA
    + value
        - JSON_TYPE : JSON_BOOLEAN
        - boolean    : 7.5