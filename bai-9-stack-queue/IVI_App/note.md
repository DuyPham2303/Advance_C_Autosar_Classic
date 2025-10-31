# Ứng dụng mô phỏng xử lý giao diện UI trong hệ thống Infotainment trong ôtô
`Mô tả`
+ Stack để quản lý điều hướng màn hình (UI navigation)
+ Queue để xử lý sự kiện từ người dùng (User Input)
+ Menu UI bằng terminal: Cho phép nhập lựa chọn từ bàn phím để mô phỏng điều hướng và thao tác
`Các tính năng`
+ Người dùng có thể chọn:
    - Chuyển đến màn hình mới (Home, Audio, Bluetooth, Settings)
    - Nhấn nút Back
    - Gửi sự kiện như "Volume Up", "Brightness Down"...
+ Hệ thống lưu màn hình vào stack
+ Các sự kiện được đưa vào queue và xử lý theo FIFO