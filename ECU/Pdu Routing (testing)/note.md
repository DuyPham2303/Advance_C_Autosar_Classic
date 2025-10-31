# Luồng điều phối dữ liệu CAN (Can Routing)
- Cách thức thông điệp CAN được 
    + giải mã để trích xuất ra tốc độ, ID 
        -> Các ECU gửi/nhận CAN msg -> cần phân tách, decode ra dữ liệu mong muốn 
    + gửi và xử lý đổng thời dựa trên cơ chế nào 
        -> nếu nhiều ECU cùng gửi msg trên bus CAN cùng lúc -> ai sẽ được xử lý trước/sau ?
# Bối cảnh
- Hệ thống xe hơi sử dụng các protocol khác nhau để truyền dữ liệu tùy mục đích CAN/LIN/FLEXRAY
    -> Các ECU khi nhận data cần xác định loại data được đóng gói dựa trên protocol nào -> gọi ra hàm xử lý, decode tương ứng
    -> Cơ chế Interrupt để phân tách độc lập quá trình truyền/nhận (chỉ khi có data mới xử lý)
# Quy trình
   ## Ngắt đọc data sau T(s)
        + call read_function() của CAN / LIN / FLEXRAY (3 task chạy độc lập)
        Ví dụ : gọi Can_ReadMainfunction() trong thread riêng kích hoạt mỗi 1s
   ## đóng gói data
       `PduR` : đóng gói dữ liệu theo chuẩn Pdu (đơn vị giao thức dữ liệu) 
                    => đã được lớp giao diện của từng protocol xử lý __(Gọi CanIf/FlexrayIf/LinIf để xử lý)__
                    => lưu trữ thành 1 gói dữ liệu tổng quát 
                    => gửi cho tầng phía trên sử dụng (không cần quan tâm kiểu giao thức cụ thể)
       `CanIf`: cơ chế xử lý xử lý dữ liệu sau khi đã pack theo kiểu PduR
        + gọi rx_indication, để giải mãi cụ thể loại protocol của gói data
       `Rx_indication` : ánh xạ source protocol chứa gói data tương ứng -> module đích (loại dịch vụ sẽ xử lý)
            + **Src type**   : nguồn protcol 
            + **dst module** : nơi data sẽ được điều hướng đến để xử lý 
                - COM : điều khiển trạng thái của xe bao gồm (set tốc độ,dừng động cơ ,xi nhan, đọc data từ cảm biến)
                - DCM : thao tác chuẩn đoán lỗi, feedback về trạng thái lỗi của xe từ black box 
                - FIM : kiểm tra các điều kiện cần thiết để cho phép thực thi lệnh yêu cầu điều khiển xe 
            * Lưu ý : mỗi tính năng nhỏ trong các module trên được phân biệt dựa trên ID
   ## gửi data đến module chức năng sẽ xử lý
        + gọi PduR_indication : điều phối gói CAN đến nơi xử lý module chức năng tương ứng __(dst module)__
            ví dụ Gọi Com_indication : phân tích ra tính năng cụ thể __(decode ID can)__
                - set speed  : tiếp tục decode data -> lấy value  
                - stop motor : cập nhật trạng thái motor để stop

# Tóm tắt quy trình gửi mã CAN và decode 
    -> call RTE -> COM -> canIF -> PduR -> driver CAN 
         
