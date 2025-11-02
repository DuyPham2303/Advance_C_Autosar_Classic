# BASIC POINTER
   **Khái niệm** 
        + ý nghĩa ? thao tác ? ưu điểm ?
        + Địa chỉ của biến tổ chức trên bộ nhớ ?
            - Bộ nhớ lưu giá trị dưới dạng nào ?
            - ý nghĩa của byte đầu - cuối -> MSB - LSB
            - So sánh với cấp phát byte và địa chỉ lưu trữ trên pointer
        => vị trí các byte của biến/con trỏ lưu ở địa chỉ thấp/cao phụ thuộc ai ? -> dẫn dắt sang endian
   **Bản chất**
        + `endian ?` 
        + `Size & datatype ?`
    **Ví dụ ứng dụng**
        + triển khai swap 2 giá trị 
        + so sánh truyền 
            - con trỏ : khi cần thay đổi
                ví dụ : hàm input -> xử lý scanf
            - giá trị : chỉ kiểm tra 
                ví dụ : hàm chứa if else để check value
   **pointer to array**
        + `thao tác ? : `
            - cấp phát các vùng địa chỉ liền kề cho các phần tử
        + `pointer arithmetic ? `
            - con trỏ dịch chuyển đến vùng nhớ với bước nhảy (số byte bỏ qua) bằng kích thước kiểu dữ liệu mà nó trỏ tới
    => Tóm tắt bài học 
        + định nghĩa : cú pháp ? dereference ? 
        + cách thức phân bố địa chỉ ?
        + endian ?
        + kích thước ? kiểu dữ liệu ? 
# POINTER VARIATION
`Void Pointer`
    **Bối cảnh:**
    + cần truy thao tác trên các biến có datatype khác nhau 
    -> khai báo bấy nhiêu con trỏ có datatype tương ứng 
    -> tốn memory => `solution tối ưu hơn ?` 
    __1 con trỏ duy nhất trỏ tớ bất kỳ datatype nào__
   
   **Triển khai:**
    + `Ví dụ:` *(datatype*)ptr : ép kiểu con trỏ để biết cần truy xuất bao nhiêu byte 
    + `demo: ` trỏ tới int,double,char,mảng ký tự (lưu ý ký tự null -> kết thúc  1 mảng ký tự)
    +` Ưu` : khai báo 1 lần duy nhất trỏ tới bất kỳ datatype
    + `nhược` : sử dụng complex -> cần ép kiểu
    **Ứng dụng**
    + Viết hàm tổng quát (Generic function) -> xử lý nhiều loại dữ liệu có kiểu khác nhau
`Function pointer` 
     **Bối cảnh:**
     + cần thao tác linh hoạt trên các function có định dạng giống nhau
    **Triển khai:**
     + <return_types> (*func_ptr)(<data_type_1>,<data_types_2>) 
     + `Thao tác`:
        -> gán địa chỉ: func_ptr = func hoặc func_pointer = &func
        => gọi hàm    : func_ptr() hoặc (*func_ptr)()
     + `demo` : 
        -> trỏ tới hàm có/không paramtinh1
        -> tính tổng/hiệu/tích/thương 3 cách 
            - trỏ từng hàm 
            - mảng con trỏ hàm chứa địa chỉ các hàm
            - hàm xử lý tổng quát thông qua truyền địa chỉ của hàm như 1 tham số 
    **Ứng dụng**
    + Trong embebedded , ứng dụng xử lý ngắt bằng cách gọi ra hàm xử lý tương ứng dựa trên địa chỉ của ISR trong vector table
`Pointer to const`
     **Bối cảnh:**
     + Thao tác kiểm tra trên vùng dữ liệu mà không muốn thay đổi 
     + Thao tác trên thanh ghi vdk (read-only) -> IDR (đọc giá trĩ trên chân GPIO cầu hình là input) 
`Const pointer`
     **Bối cảnh:**
     + thao tác trên 1 thanh ghi duy nhất có địa chỉ cố định (ko được phép trỏ đến địa chỉ khác)
     **Ứng dụng**
     + kết hợp volatile trong khai báo thanh ghi
     + kết hợp cả 2 loại const ptr và ptr to const
`Null pointer`
     **Bối cảnh:**
     + Khai báo 1 con trỏ mà không gán địa chỉ cụ thể
      -> có thể trỏ ngẫu nhiên tới địa chỉ của vùng nhớ khác
      -> nếu truy xuất có thể làm ành hưởng thay đổi dữ liệu (ko an toàn)
     **Ứng dụng**
     => tránh việc con trỏ tới vùng nhớ ngẫu nhiên trong chương trình (ko an toàn) -> cần gán NULL khi khai báo 
     __Lưu ý__ : ko nên truy xuất null ptr vì đã trỏ tới địa chỉ ko chứa dữ liệu -> program có thể warning
`Double pointer`
      **Bối cảnh:**
      + cần thay đổi giá trị của 1 con trỏ thông qua con trỏ khác
      **Ứng dụng**
      + Cần thao tác phân tách dữ liệu trên chuỗi nhận về dưới các định dạng như json, để lấy ra thông tin mong muốn 


   
