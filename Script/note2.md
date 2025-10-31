### STACK FRAMES and GDB
+ Giải thích về GDB
    => demo chạy thử bằng công cụ debugger (GDB) 
    => chạy lại gdb thủ công bằng command
        -> các lệnh thao tác :
        + gdb file.exe : vào chế độ chạy debug
        + run : chạy toàn bộ program
        + start : vào chế độ debug từng dòng
        + where: trả về địa chỉ của dòng lệnh hiện tại
        + disassemble main : in ra phần biên dịch của hàm main thao tác trên thanh ghi
        + list : show các dòng code từ dòng debug hiện tại
        + quit : thoát khỏi chế độ gdb
    
+ Giải thích về PC 
    => con trỏ tới lệnh tiếp theo sẽ được thực thi
+ Triển khai 1 chương trình gọi hàm cơ bản 
    => giới thiệu cách mà hàm đó được xử lý như thế nào 
    => vẽ ra sơ đồ stack frame
    <!-- 
        - cách mà 1 hàm được gọi sau khi thực hiện xong lại trở về main
        - khi 1 hàm được gọi 

            => return add được push vào Stack frame => nhờ đó mà con trỏ chương trình biết vị trí quay về sau khi hàm thực thi xong
     -->
### CALLING CONVENTION

Bộ nguyên tắc gọi hàm 
**quản lý nơi lưu trữ các**
    + các bién cục bộ
    + hàm con
    + đệ quy
    -> tên các register , stack 
**Sử dụng lệnh : push/pop/call/ret**
    => caller : nơi gọi hàm
    => callee : code inside hàm 
CALLER'S RULE
    **before calling function**
    + caller backup data -> EBX,ECX,EDX -> push Stack
    + đẩy param từ right -> left
        => do stack expand từ high -> low address
        => ensure last in first out 
    **calling function**
    + use instruction call
        => push return add to top stack
        => go inside and execute code line
    **escape function**
        => caller xóa param 
        => khôi phục top stack (update SP)
        => caller get return value in EAX 
        => caller pop giá trị trên EBX,ECX,EDX __(khôi phục trạng thái chương trính)__
CALLEE'S RULE
    **Push EBP to Stack**
        meaning: cho phép truy cập các địa chỉ local param (bảng đồ luu ví trí)
        reason : backup EBP tránh function execution thay đổi EBP của caller
        => copy giá trị của SP vào BP , cho phép acces param và local var
        => cấp phát vùng nhớ cho local, bằng cách mở rộng stack từ address high -> low
        => 
CÁC PHẦN CÒN LẠI

**Câu hỏi**

+ Tại sao lưu lại old frame pointer (BS) vào stack và gán BS = SP
+ Tại sao gán RSP = RBP, trước khi giải phóng local var
+ Việc xử lý tham số hàm là đẩy vào stack (nếu không đủ thanh ghi cho phép), có quy định về thứ tự thao tác không ?
+ Thanh ghi EDI, ESI làm gì ? quản lý bới ai ?
+ Tại sao RBP lại giúp tham chiếu và truy cập các local var và param
+ callee có giải phóng local var hay caller sẽ làm điều đó ?
+ RBP, EBP , hay BS mới là cách gọi đúng 
**Nội dung**

+ Triển khai giúp tôi Ví dụ cho học viên (beginner)
 => lỗi stackoverflow, stagmentation fault để làm rõ vai trò của stack pointer
 => lỗi sai return address, sai thứ tự truyềm param, đẻ làm rõ vai trò của calling convention


