# STDARG
## TRÌNH BÀY BỐI CẢNH 
    - Ví dụ triển khai trên chương trình quản lý 1 danh sách/tập dữ liệu ko xác định số lượng 
    - liên hệ hàm printf 
## Ví dụ Triển khai hàm xử lý nhiều tham số (kiểu khác nhau)
    - so sánh hàm xử lý function(int count,....) vs printf -> khác gì ?
        + các thành phần của hàm xử lý
        + Các macro xử dụng 
`va_list`: 
    + con trỏ kiểu char
    + lưu trữ chuỗi 
        -> scần trích xuất 
        -> tách ra các đối tượng cần xử lý 
`va_start` : 
    + xác định vị trí bắt đầu tách dữ liệu trong args
`va_arg`
    + lấy ra từng phần tử __(cần ép kiểu để lấy ra giá trị mong muốn)__
`va_end` : 
    + thu hổi danh sách đối số
## Ví dụ tính Sum bằng stdarg 
    - giả dụ 1 macro để tính tổng -> hạn chế ?
    + Cách 1: sử dụng biến count 
        => nhược điểm : số lượng đối số xử lý phụ thuộc đối số đầu 
    + Cách 2: sử dụng giá trị quy định list args
        => nhược điểm : phụ thuộc vào giá trị 
    + Cách 3: Sử dụng chuỗi bất kỳ để quy định list args 
**Note**
<!-- 
    - cần ép kiểu trả về là char* trong va_arg -> value
    - nếu result += value, vói result là int -> vẫn sai (do tự động chuyển thành ascii, để tính) -> "\001" -> '1' -> 48
    - nếu result += (int)value -> vẫn sai do
    - cần (int)`\1` -> 1
    - sử dụng 1 biến trung gian check (va_copy) để sao chép dữ liệu của args gốc (va_list)
    - chỉ cần 1 va_end (vì check sao chép args -> args giải phóng -> check cũng mất)
    -->
## Ví dụ thực tế (Xử lý dữ liệu nhiều cảm biến)
    - khai báo danh sách enum các tên cảm biến
    - hàm xử lý : 
        + tham số 1 quy định loại sensor
        + tham số 2 quy định kiểu dữ liệu sensor (đơn vị tương ứng)
    - switch case xử lý tách và đọc ra dữ liệu tương ứng với sensor
# ASSERT
## lý thuyết
    - tổng quan (slide)
    - so sánh với if else (nhược điểm ? -> ko biết vị trí xủ lý)
## Ví dụ
    - demo code -> minh họa
        + ưu điểm assert 
        + nhược điểm if else
    - viét macro tổng quát xử lý (Variadic)
    - Tổng kết so sánh if-else và assert
        + assert : 
            -> in ra điều kiện và thông báo (vị trí code line,file)
            -> dừng program
        + if-else :
            -> chỉ in ra thông báo lỗi
            -> tiếp tục xử lý flow program 
        
    