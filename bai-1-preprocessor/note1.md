# Dẫn dắt 
    + Khi bấm Run -> file nguồn -> file.exe -> làm thế nào ?
    + Giới thiệu sơ lược IDE (ví dụ: arduinoide,stm32cubeide) 
        + Editor  : trình soạn thảo -> chứa giao diẹn để ghi mã nguồn  
        + Compiler : mỗi ngôn ngữ có compiler khác nhau -> avr-gcc (arduinoIDE) 
# Giới thiệu compiler : 
    + là gì ? tại sao phải dùng ? dùng như thế nào ? => xử lý qua các bước (ví dụ)  
   ## bối cảnh 
        => cầu nối giữa máy và người (mã nguồn -> thực thi)
        => hỗ trợ tối ưu và phát hiện lỗi (logic,cú pháp,khai báo)
   ## demo run program
        -> sinh ra file .exe (kết quả của việc chạy chương trình)
        -> quá trình xử lý qua nhiều step bên trong máy tính
        -> các bước ?
# Preprocessing
   ## Ý NGHĨA
        - Quá trình mở rộng mã nguồn C bằng cách thực hiện
            + chèn nội dung các tệp được chỉ định trong #include
            + thay thế các macro được định nghĩa với #define
            + xóa các comment
            + xử lý các chỉ thị biên dịch có điều kiện, bằng cách đánh giá giá trị của các macro liên quan
   ## TRIỂN KHAI CHI TIẾT 
        
   ` #include` 
        - 
   ` #define và #undef `
        **macro** : là một loại chỉ thị tiền xử lý, được định nghĩa bằng #define, trước khi chương trình biên dịch
        **#define** : chỉ thị thay thế, được dùng để thay thế 1 đoạn văn bản hoặc chuỗi ký tự hoặc biểu thức bằng 1 đoạn văn bản khác để dại diện cho ý nghĩa cụ thể của chúng
   `#,##,variadic`
	    - # và ## là gì ? -> demo code 
        - variadic ?
          + tính sum thông thường ? -> cách tổng quát hơn ? -> nhận vào n đối số -> variadic 
          + chú thích … và __VA_ARGS__ ? 
          + ví dụ tính sum -> sử dụng điều kiệu dừng (giá tri 0) và tính Size 
   `(biên dịch có điều kiện)`
    **#if / #elif / #else / #error**
       - so sánh (if,else) 
           + kiểm tra biến ? -> logic if else
           + kiểm tra điều kiện macro ? -> #if #else
           => ví dụ MCU :  cấu hình phần cứng cho nhiều loại MCU khác nhau (tốc độ,mode,vv..) 
    **#ifndef / #ifdef / #endif / #error** 
       - Ví dụ về define SIZE cho array
       - Thiết kế lib
            + demo include .c multiple time -> issue ?
            + show standard lib 
            + tạo .h với include guard với .c tương ứng
# Compilation 
dịch mã tiền xử lý sang mã trung gian, để thực hiện phân tích cú pháp, và ngữ nghĩa của chong7 trình nhằm quyết định cách
    + Ánh xạ và lưu trữ các biến vào bộ nhớ
    + Tối ưu hóa mã lệnh và sinh ra chỉ dẫn tương ứng để thao tác thên thanh ghi hoặc vùng nhớ
# Assembler : 
Dịch mã trung gian sang mã máy (machine code - binary), tạo ra tệp object chứa các mã lệnh có thể hiểu bởi cpu
# Linker :
Liên kết nhiều tệp mã máy lại để tạo ra tệp thực thi mà chương trình chạy được, nó xử lý các bước
    + Liên kết các thư viện hệ thống và tệp header
    + Tìm và liên kết lời gọi hàm và định nghĩa của chúng
    + Báo lỗi nếu gọi hàm chưa được định nghĩa 
# CHẠY CHƯƠNG TRÌNH
    + chạy các lệnh 
    + tóm tắt nội dung bài học
----------------------------------------
# CÁC BƯỚC GIẢNG DẠY
----------------------------------------
## Mở đầu
- trình bày bối cảnh chạy program -> tìm hiểu cụ thể quy trình -> 
- Trình bày về công cụ lập trình IDE -> 2 thành phần Editor và compiler 
- Nói rõ compiler ? 
## Nội dung bài học
### TIỀN XỬ LÝ 
- chạy code trước phần tiền xử lý -> so sánh 2 bên
- giải thích khác biệt từng thằng 
- giới thiệu sơ lược macro 
=> kết luận quá trình tiền xử lý làm gì 
- Tìm hiểu sâu về macro -> gồm các loại gì
    + `include` -> <> và "" khác gì nhau
    + `macro` : ví dụ trên slide 
    + `#if #else #elif #error`
        -> so sánh với if-else thông thường 
        -> kiểm tra điều kiện biến
        -> chốt lại chức năng của directive 
        -> lái sang ví dụ mô tả (ứng dụng lựa chọn đoạn lệnh xử lý dựa trên MCU yêu cầu)
     + `#ifdef , #ifndef` 
        -> giải thích code trong main
        -> lái sang thiết kế lib (ko include .c)
        -> so sánh với thư viện hệ thống (lý do cần có include guard ?)
- Build & run với file nguồn có include .h -> lỗi -> vì sao ? (ko tìm thấy định nghĩa -> thiếu liên kết) -> 
### triển khai các giai đoạn còn lại
### giới thiệu macro mở rộng
- macro nối chuỗi : ứng dụng tạo ra 1 tên biến bằng cách ghép các đoạn văn bãn
- macro chuẩn hóa chuỗi : chuyển đổi text -> chuỗi
- macro variadic : ví dụ dẫn dắt giả sử 
    #define sum(a,b,c) a+b+c 
    -> chỉ khả thi nếu input xác định trước 
    -> cần cách tổng quát để xử lý  
=> Ví dụ áp dụng thực tế 
### GDB 
- Ví dụ debug nhiều loại lỗi > debug bằng GDB