# Tài nguyên là gì 
- Vùng nhớ chia sẻ chung giữa nhiều luồng với nhau
    + Biến toàn cục
    + dữ liệu xử lý bên ngoài như file, hàm stdout,stdint (đọc bản phím)
    + thiết bị ngoại vi như màn hình (terminal,lcd)
    => Yêu cầu đồng bộ giữa các luồng dể xử lý tránh các lỗi như race condition 
# atomic 
    - cơ chế đảm bảo các luồng truy cập dữ liệu chung an toàn -> bảo vệ dữ liệu ko bị tác động bởi nhiều luồng
    - Áp dụng cho biến đơn, xử lý tính toán đơn giản
    - chủ yếu dùng để đọc/ghi status/data giữa các luồng
`Nhược điểm`
    - Không áp dụng cho dữ liệu/tính toán phức tạp
        + cấu trúc struct
        + xử lý tính toán nhiều bước, câu lệnh
# mutex
    - Cơ chế đảm bảo các luồng truy cập tài nguyên chung 
    an toàn, -> bảo vệ nguồn tài nguyên chỉ được 1 luồng
    được phép truy cập tại 1 thời điểm
    - Áp dụng cho tài nguyên chung (ngoại vi terminal,bàn phím, màn hình)
`Nhược điểm`
    - kết hợp với atomic để thao tác với dữ liệu/các bước xử lý phức tạp
=> Tóm lại giúp ngăn chặn nguy cập trái phép -> ko giúp các luồng giao tiếp với nhau (nếu cần thiết lập cơ chế signal/wait giữa các luồng)
# condition variable 
## Cơ chế condition
- cơ chế cho phép 1 luồng đợi sự kiện kích hoạt từ luồng khác mới được phép chạy giúp
    + tối ưu thao tác kiểm tra liên tục -> Giảm bớt gánh 
      nặng cho CPU 
      -> CPU có thể đi làm việc khác mà không
      cần kiểm tra trạng thái dữ liệu
      -> chỉ tiến hành xử lý khi có tín hiệu từ luồng khác
- sử dụng 2 cơ chế
    + wait   : luồng chờ tín hiệu 
    => có thể tạm thời được đưa vào trạng thái sleep, và 
    được đánh thức khi có signal
    + signal/broadcast : luồng thông báo tín hiệu
    => xử lý công việc và thông báo cho luồng chờ tín hiệu
## Bối cảnh
- Ví dụ ta có 2 luồng để đọc - xử lý dữ liệu cảm biến 
    + Producer -> sensor : đọc dữ liệu
    + Consumber -> ECU   : xử lý tính toán
    => nếu dùng for kiểm tra -> CPU chạy liên tục -> lãng phí