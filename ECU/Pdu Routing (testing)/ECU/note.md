# Vấn đề
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
   **Khởi tạo ban đầu**
        + Khởi Can Driver, COM Stack (CanIf,PduR,COM) 
   **Ngắt đọc data định kỳ** 
        + Gọi hàm đọc frame data từ mcal tương ứng loại protocol (can/lin/flexray)
   **đóng gói Pdu** :  
        + Mỗi loại protol triển khai xử lý frame và phân tách ra Id,dlc,data
        + đóng gói lại theo chuẩn Pdu (thống nhất về 1 tiêu chuẩn duy nhất để đóng gói dữ liệu truyền thông)
        + Gửi lên lớp giao diện để xử lý gọi Rx_Indication tưng ứng
    **Kiểm tra gói PdU hợp lệ**
       + lọc ID (xác nhận loại thông điệp nào được phép xử lý tiếp)
       + Gán nhãn Protocol cụ thể cho gói PdU (cho phép tầng trên điều phối đến logic xử lý chức năng mong muốn)
       + Gửi gói PdU cho phép xử lý tiếp tục 
    **Định tuyến đến module xử lý chức năng cụ thể**
        + nhận gói PdU từ tầng dưới (CanIf.LinIf,FlexrayIf) đã xử lý 
        + Tiến hành gọi module tương ứng dựa trên 2 thông tin
            + **Src type**   : nguồn protcol gửi gói PdU
            + **dst module** : nơi data sẽ được điều hướng đến để xử lý 
                - `COM` : điều khiển trạng thái của xe bao gồm (set tốc độ,dừng động cơ ,xi nhan, đọc data từ cảm biến)
                - `DCM` : thao tác chuẩn đoán lỗi, feedback về trạng thái lỗi của xe từ black box 
                - `FIM` : kiểm tra các điều kiện cần thiết để cho phép thực thi lệnh yêu cầu điều khiển xe 
            * Lưu ý : mỗi tính năng nhỏ trong các module trên được phân biệt dựa trên ID
   **Decode data và gửi cho Rte** 
        + Các module (Com,Fim,Dcm) nhận gói PdU và Id 
        + tiến hành điều phối đến logic tương ứng dựa trên Id để decode loại data cụ thể từ PdU
        + Gửi data (chứa giá trị thực tế) thông qua giao diện Rte
        ví dụ Gọi Com_indication : phân tích ra tính năng cụ thể __(decode ID can)__
            - set speed (Id = 0x03) : tiếp tục decode data -> lấy value  
            - stop motor(Id = 0x04) : cập nhật trạng thái motor để stop
# Tóm tắt quy trình CAN routing
   + Interrupt Task : loop 10ms -> Can_MainFunction_Read() (MCAL)
   + Can Routing : MCAL -> CanIf -> PdUR -> COM 
        - MCAL  : Package PduInfoType 
        - CanIf : Decide whether PdUIntoType continue process or not 
        - PdUR  : Route PdU package to Com module
        - Com   : Implement decode type of data based on Id -> publish to Rte
            + Periodic called : update RTE
            + Oncetime called : Init CanIf & PduR
                - CanIf -> Init Can -> configure Can's param
                - PduR  -> configure number of streaming route (Luồng định tuyến đến chức năng mong muốn) (depends on number of ECU implement communication on CAN bus)
# Thiết kế Sequence Diagram
+ 2 giai đoạn : Khởi tạo và loop 10ms
+ Các module  : Com , PduR , Can driver
+ Cụ thể
    - Giai đoạn 1 : Com_Init() call Can_Init() & PduR_Init()
    - Giai đoạn 2 : Loop 10ms 
        **Các bước xử lý**
        - Read can's frame -> Package Pdu -> Proceed Valid PdU package -> Route to module Com -> Com publish to Rte
        **Quy trình gọi hàm**
        Can_MainFunction_Read() --call--> CanIf_RxIndication(CanRxPduId,&rxPdu) --call--> PduR_RxIndication(srcProtocol,srcPduId,PduInfoPtr) --call--> Com_RxIndication(uRxPduId,PduInfoPtr) --call--> Rte_Com_Update_EngineSpeed(rpm)