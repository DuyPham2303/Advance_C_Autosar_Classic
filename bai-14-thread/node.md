# Bối Cảnh
- Cách thức code tuần tự
    + ưu điểm
    + nhược điểm
    => 2 hàm chạy khác tính năng cần chạy độc lập & song song
+ So sánh chạy // và tuần tự
    PC 
        -> chạy từng câu lệnh của mỗi hàm 
        -> chạy xen kẽ từng câu lệnh của nhiều hàm cùng lúc 
# Process & thread
    -> tiến trình (app chạy trên máy tính) 
        + chứa nhiều luồng bên trong (ví dụ mở nhiều tab trên chrome)
        - Luồng : 
            + chạy // và độc lập vs nhau 
            + đơn vị nhỏ nhất của tiến trình
# Demo 
    + chạy 2 luồng đếm biến count
    + chạy 2 luồng kèm param truyề vào (in ra chuỗi hoặc value)
    + chạy thêm ứng dụng trong main loop
        -> vấn đề khi bỏ main loop (luồng chính) -> các luồng khác cũng dừng do phụ thuộc (Outline hình minh họa)
        -> giải pháp ? -> luồng chính chờ cho các luồng phụ thực thi xong mới end
    + Sử dụng hàm join chạy 2 trường hợp : so sánh khác biệt 
        + t1 (loop n lần) -> t2 (loop liên lục) -> main thread
        + t1 (loop liên tục) -> t2 (loop n lần) -> main thread
    