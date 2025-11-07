PHẦN 1 - BASIC 
# Đặt vấn đề dẫn đắt
   ## ví dụ hệ thống quản lý thông tin user , để khảo sát 
        + phương tiện di chuyển : ô tô,tàu,xe máy vv...
        + trạng thái : 1 mình / gia đình ?
        + datatype phù hợp ? tiết kiệm ? 
        => cân nhác đến bộ nhớ RAM
   ## kích thước datatype ?
        + lib stdint.h (cung cấp các kiểu tối ưu) -> khảo sát từng datatype
        + cách tối ưu nhất (dùng 1 bit - tránh waste memory)
  => dẫn đến khái niệm bitmask (thao tác trên từng bit để lưu trữ thông tin) => ứng dụng 
#  Phép toán LOGIC
  ## AND,OR,NOT,XOR
    + lưu ý về NAND,NOR, phép ^ và hàm pow 
  ## dịch bit 
    + right/left shift
    + lưu ý dịch số âm
    + demo ví dụ slide
  ## macro define 
    + quy định status của 1 đối tượng -> bit muốn làm việc
# Triển khai các thao tác bitmask
  ## viết các hàm 
    + enableBit , disableBit, checkBit (minh họa AND,OR,NOT)
    + pass by value vs pointer ? 
    => Kết luận so sánh 
        + enable/disable bit : pass by pointer -> change value
        + check bit: pass by value -> unchanged value 
    + enable multi-bit ? -> truyền vào hàm thông qua phép OR
   ## demo ví dụ quản lý lựa chọn tính năng 
PHẦN 2 - ADVANCE
# Ví dụ và ứng dụng XOR
  - hoán đổi 2 số 
    -> cách thông thường : dùng biến tạm -> lái sang cách thao tác bit
  - Áp dụng viết hàm GPIO_TogglePin
  
# Ứng dụng quản lý thông tin cấu hình của xe
   ## Phạm vi giá trị để dùng số bit phù hợp ?
        + chỉ định bao nhiêu bit để lưu dải giá trị mong muốn
        uint8_t,uint16_t,uint32_t -> 2^n - 1 giá trị 
        -> 2 bit : 2^2 = 4 giá trị
        -> 3 bit : 2^3 = 8 giá trị 
   ## bit field (quy định số bit cụ thể) 
        + Khái niệm
            - chỉ định số bit cụ thể sẽ sử dụng nhằm tiết kiệm bộ nhớ 
            - Áp dụng trên struct 
            - member khai báo liên tiếp có cùng datatype share common memory address
            - Nếu số lượng bit lưu trữ vượt quá datatype, member kế tiếp sẽ được cấp phát địa chỉ mới 
        + có đọc được địa chỉ ? : không đọc được, do ô nhớ bị chia nhỏ 
        + những bit thừa đi đâu ? 
            - ko bị xóa hay cắt từng bit -> vẫn được cấp phát theo đơn vị tối thiểu mà CPU vẫn truy cập được 
            - được quy hoạch để chỉ định bit nào thuộc về trường nào
            - Compiler đặc các trường bit field vào vị trí bit cụ thể 
            - Các bit thừa bị bỏ trống - ko bị xóa 
            - CPU khi đọc/ghi 
                -> thao tác nguyên byte/word 
                -> sau đó mask để lấy đúng phần bit field
            Ví dụ 
            struct {
                a : 1;
                b : 2;
                c : 1;
            }field;
        Bit index   7       6       5   4   3   2   1   0
        nội dung  padding padding   c   b   b   a   a   a
    => 2 bit cuối cùng là byte đệm, không bị xóa -> compiler đơn giản bỏ qua 
        + ảnh hưởng bit field (cách khai báo , thứ tự , kiểu)
        + khi nào sử dụng ? 
-------------------------------------------------------------------
trình bày ví dụ dẫn dắt 
trình bày về việc tiết kiệm RAM
trình bày về ứng dụng trong lĩnh vực nhúng 
trình bày về các cổng logic 
trình bày về phép dịch bit và demo thao tác 
trình bày về ví dụ kết hợp dịch bit + macro -> bitmask
trình bày sơ lược bật/tắt bit để lựa chọn tính năng
trình bày về các hàm thao tác bit
trình bày ví dụ cụ thể về quản lý lựa chọn trang phục
trình bày về ví dụ liên quan đến `bit field `
trình bày về hoán đổi 2 số với `phép XOR`
trình bày về ví dụ mô phỏng `truyền/nhận dữ liệu MCU`
     
