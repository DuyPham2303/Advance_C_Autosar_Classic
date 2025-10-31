# Bối cảnh dẫn dắt
    - data structure ? -> ứng dụng ?
    - loại cơ bản ? -> mảng -> dùng how ? -> cấp phát, địa chỉ ? -> loại data structure ?
    - giới thiệu 2 loại linear và non-linear
# Stack (Ngăn xếp)
   ## Ngăn xếp vs phân vùng stack ?
   - Ngăn xếp deprived from -> stack -> cơ chế LIFO
   - Khác biệt ? 
        `Phân vùng stack`   
            + Os cấp phát/quản lý 
            + lưu local var,param
            => lưu data short-term
        `Ngăn xếp`
            + User quản lý
            + Lưu nhiều loại data ở stack/heap/global
            => lưu data long-term
    => Minh họa LIFO qua ví dụ thêm sách vào hộp
    ## Các thao tác trên Ngăn xếp
        - Push,pop,peek/top -> giải thích dựa trên ví dụ minh họa
        - Quản lý dữ liệu phối hợp với mảng -> mảng truy cập thế nào ? -> chỉ số 
    ## so sánh chỉ số (mảng) và top (ngăn xếp)
       `chỉ số `
            + truy cập phẩn từ mảng
            + không ngăn chặn truy cập __out-of-bound__
       `top`
            + truy cập phần tử đỉnh ngăn xếp
            + cho biết status (đầu/rỗng) -> quản lý dữ liệu hiệu quả, an toàn 
    ## Mối liên hệ giữa top và push/pop
        - top cho biết status để push/pop phù hợp
        - minh họa ví dụ 
            + stack rỗng : top = -1
            + stack đầy  : top = size - 1
        - top thay đổi ntn khi push/pop ?
    ## Tóm lại 4 đặc điểm
        - khi push/pop/empty/full -> stack => top thế nào ?
    ## Triển khai code 
        - Xây dựng cấu trúc và các hàm thao tác
        - ví dụ tính giai thừa bằng cơ ché push/pop trên ngăn xếp

# QUEUE
   ## Bối cảnh dẫn dắt
        + Ví dụ về hàng người xếp hàng chờ mua cafe
        + theo thứ tự ai vô trước thì được mua trước
        => co chế hoạt động FIFO
   ## sO SÁNH Ngăn xếp
        + Các thao tác thêm/xóa
        + phần tử truy cập 
        + kiểm tra empty/full
        + các loại 
        => Tổng kết đặc điểm qua ví dụ đầu
    