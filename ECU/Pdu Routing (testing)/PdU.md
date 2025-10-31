- Trong AUTOSAR, mô-đun PdU (Protocol Data Unit Router) là một thành phần quan trọng trong ngăn xếp giao tiếp (Communication Stack), đóng vai trò như một bộ định tuyến để chuyển tiếp các đơn vị dữ liệu giao thức (PDU) giữa các mô-đun giao tiếp khác nhau. 
**Vai trò của PDU trong AUTOSAR**
    + Trừu tượng hóa tín hiệu: PDU giúp ẩn đi chi tiết của công nghệ truyền dẫn cơ bản, cho phép các ứng dụng (thực thể vật lý của ECU) làm việc với tín hiệu mà không cần 
    biết chúng được truyền đi như thế nào. 
    
    + Độc lập với công nghệ: Các PDU có thể di chuyển qua các mạng lưới sử dụng công nghệ khác nhau (ví dụ: CAN FD, Ethernet ô tô) mà không cần thay đổi cấu trúc. 
    
    + Tích hợp tín hiệu: Nhiều tín hiệu từ các nguồn khác nhau có thể được gom lại và đóng gói vào một PDU để truyền đi, sau đó được giải nén ở phía nhận. 
**Chức năng chính**
    + Định tuyến PDU: Chuyển tiếp các PDU đến đúng mô-đun đích dựa trên ID PDU và bảng định tuyến đã được cấu hình.
    
    + Chuyển tiếp cổng (Gateway): Cho phép truyền dữ liệu giữa các giao thức giao tiếp khác nhau, ví dụ: định tuyến PDU từ bus CAN sang bus LIN hoặc Ethernet.
    
    + Trừu tượng hóa giao tiếp: Cho phép các mô-đun phía trên trong ngăn xếp giao tiếp (như COM) độc lập với giao thức truyền thông cụ thể (như CAN, FlexRay), từ đó tăng tính linh hoạt và khả năng tái sử dụng của phần mềm. 
**Cơ chế hoạt động**
    + PduR hoạt động như một lớp trung gian giữa các mô-đun lớp trên (chẳng hạn như COM và DCM) và các mô-đun lớp dưới (chẳng hạn như CAN IF, LIN IF hoặc CAN TP). 
    
    + Định tuyến lên (Upstream routing): Khi nhận được PDU từ một mô-đun lớp dưới, PduR sẽ chuyển tiếp nó đến đúng mô-đun lớp trên dựa trên bảng định tuyến.
    
    + Định tuyến xuống (Downstream routing): Khi một mô-đun lớp trên yêu cầu truyền PDU, PduR sẽ định tuyến PDU đó đến mô-đun lớp dưới phù hợp. 

**Các loại PDU được xử lý**
`I-PDU (Interaction Layer PDU):` 
    + Các PDU chứa dữ liệu ứng dụng (tín hiệu) được tạo và xử lý bởi mô-đun COM.
`N-PDU (Network Layer PDU):` 
    + Các PDU được sử dụng bởi các mô-đun giao thức truyền tải (Transport Protocol), chẳng hạn như CAN TP, để xử lý các tin nhắn lớn hơn kích thước PDU tiêu chuẩn.
`L-PDU (Data Link Layer PDU):` 
    + Các PDU được sử dụng bởi các mô-đun giao diện phần cứng (Communication Interface), chẳng hạn như CAN IF, để tương tác trực tiếp với bộ điều khiển giao tiếp. 
**Vai trò trong giao tiếp liên ECU**
    + Khi giao tiếp trong cùng một ECU, PduR định tuyến PDU giữa các mô-đun nội bộ của ngăn xếp giao tiếp.
    
    + Trong giao tiếp giữa các ECU, PduR là thành phần cốt lõi của chức năng cổng, cho phép dữ liệu được chuyển tiếp từ một bus vật lý sang một bus khác, đảm bảo giao tiếp liền mạch trong toàn bộ mạng xe.
**Mô-đun COM và PDU**
    + Đóng gói/Giải nén tín hiệu: Mô-đun COM (Communication) chịu trách nhiệm lấy các tín hiệu cần truyền và đóng gói chúng vào một PDU. Ngược lại, nó cũng giải nén các tín hiệu từ PDU nhận được.
    
    + Quản lý khởi tạo: Mô-đun COM khởi tạo các PDU khi hệ thống bắt đầu hoạt động (Com_Init), điền các vùng chưa sử dụng bằng giá trị cấu hình và khởi tạo các tín hiệu ở phía máy phát và máy thu.
    
    + Lọc tín hiệu: Mô-đun COM có thể áp dụng các bộ lọc ở phía máy thu để bỏ bớt các tín hiệu không cần thiết, giúp giảm tải cho hệ thống.