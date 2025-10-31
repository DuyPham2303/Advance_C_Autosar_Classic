# Bối cảnh 
- Ý tưởng để giúp các phương tiện khi tham gia giao thông trên đường phố trở nên an toàn hơn và phòng tránh tai nạn đó là phát triển công nghệ cho phép các xe có thể giao tiếp với nhau thông qua hệ thống mạng.
- Ví dụ đơn giản như sau
    + xe A gửi msg "xe đang phanh" 
    + xe B đi ngay sau A có thể nhận msg trên, nhờ đó có thể giảm tốc 
# Vấn đề 
- Tuy nhiên việc giao tiếp giữa các xe có thể bị can thiệp và giả mạo thông điệp trao đổi bởi hacker nhằm gây tai nạn
=> chính vì vậy ta cần phát triển giải pháp để đảm bảo
    + Msg được gửi từ đúng xe đang gửi 
        -> ngăn chặn hacker gửi fake msg
    + Msg cần được đảm bảo toàn vẹn 
        -> ko bị thay đổi bởi hacker
    + Msg cần được mã hóa 
        -> ngăn chặn hacker nghe trộm nhưng ko hiểu nội dung
# SHA256 - ECDSA 
## Đặc điểm
    - Đảm bảo sự tin cậy của msg (toàn vẹn)
    - Đảm bảo msg luôn được xác thực (cho biết chính xác xe nào gửi)
## Cơ Chế hoạt động
    - Xe A khi gửi msg sẽ xử lý 
        `(SHA256)`
        + Tạo bản tóm tắt từ dữ liệu gốc **(hash)**
            => băm dữ liệu gốc thành một giá trị hash cố định (digest).
        `(ECDSA)`
        + Tạo chữ ký số **digital signature** 
            => dùng private key để ký lên giá trị hash này.
        + tiến hành truyền đi    
    - xe B khi nhận msg sẽ xử lý 
        `(ECDSA + SHA256)`
        + dùng public key để xác minh chữ ký nhằm đảm bảo
            -> msg vẫn toàn vẹn, ko đổi (integrity)
            -> msg được gửi từ đúng xe A hợp lệ (authentication)
## Tóm tắt
ECDSA-SHA256 là một cơ chế chữ ký số (digital signature) kết hợp giữa:
- ECDSA : Thuật toán chữ ký số bất đối xứng 
    -> dùng để xác thực `tính toàn vẹn và nguồn gốc dữ liệu`.
- SHA-256 : thuật toán băm mật mã
    -> tạo ra `chuỗi 256 bit đại diện duy nhất` cho dữ liệu
# EAS 
## Đặc điểm
- đảm bảo msg được mã hóa tránh nghe trộm `(Confidentiality)`
## Cơ Chế hoạt đổng
    - xe A gửi msg 
        + mã hóa msg bằng AES-128
        + tạo tag MAC (AES-CMAC / HMAC-SHA256)
    - Xe B nhận
        + giải mã bằng AES-128
        + Kiểm tra MAC bằng cùng khóa 
        + Nếu đúng -> msg hợp lệ
## Tóm tắt
- AES : Thuật toán mã hóa đối xứng, dùng chung 1 khóa cho mã hóa/giải mã
- Biến thể : 128/192/256 bit key
# LỢI ÍCH 
- Đảm bảo dữ liệu trao đổi 
    -> không bị fake giả mạo `(authenticity)`
    -> không bị sửa đổi - toàn vẹn `(integrity)`
    -> không bị nghe lén    `(confidentiality)`
# Luồng xử lý minh họa
`Người viết thư : `
    + viết tóm tắt nội dung (SHA-256)
    + Ký tên lên bản tóm tắt (ECDSA)
    + Niêm phong thư (AES) 
    + Gửi đi
`Người nhận thư : `
    + mở thư và đối chiếu chữ ký (ECDSA - SHA256) -> đảm bảo
        - tin cậy : gửi từ đúng đối tượng 
        - toàn vẹn : không bị thay đổi
    + Mở niêm phong thư và đọc nội dung (EAS)
        - 