# HTTP là gì
+ 1 phần trong tập giao thức TCP/IP(triển khai dựa trên Socket -> cầu nối giao tiếp 2 chương trình trên thiết bị khác nhau)
+ hoạt động theo mô hình clien-server với cơ chế __request - repsonde__
+ Quy định
    - cấu trúc message
    - method send/receive via client-server
    => Truyền dữ liệu siêu văn bản (text,image,sound,video) => mã hóa thành giao diện hiển thị trên web
# Mô hình HTTP
+ user truy cập website via trình duyệt -> session được tạo ra với 
    - client : user's computer
    - server : máy chủ website
    - port 80 : cổng mặc định kết nối 
+ Các bước thiết lập 
    - client initiate TCP connection -> server
    - Socket interface (kết nối được thiết lập) bao gồm:    
        => IP address
        => protocol type 
        => port number
    - kết nối thành công -> client request server với:      
        => request message : chứa path 
    - server nhận và handle request qua socket -> gửi client response 
    - data return là file HTML
    - Server đóng TCP connection
    - Client nhận response từ server -> đóng TCP connection
# Request message 
+ Cấu trúc gồm:
    __Request Line__
        - method : GET , POST, HEAD, PUT , DELETE
        - URL (path) : /index.html 
        - protocol version : HTTP/1.1 hoặc 1.0
    __Header__ 
        - format : "Name:Value" 
    __Body__
        - data client -> server
        - empty body -> use GET
        - content body -> use POST, PUT
# Response message
   __Status line__
        - protocol version
        - Status code; 200,404,302
        - Status text: 200 OK, 404 Not Found, 403 Forbiden
   __Header line__
        - format : "Name:Value"
   __Body__
        - data return ở dạng 1 trang HTML : giao diện web hiển thị 



# Bước 1 – Viết một HTTP server đơn giản
nếu client gửi GET /, server sẽ trả về HTML chào mừng.
# Bước 2 
+ Server đọc nội dung file index.html thật từ ổ đĩa.
+ Gửi nội dung file đó về cho client khi truy cập /.
<!-- 
        Đọc file văn bản bằng C:

        fopen(), fread(), fclose()

        Đọc toàn bộ nội dung vào char *html

        Gửi Content-Length chuẩn bằng strlen(html)
 -->
# Bước 3:
+ Hiểu cách phân tích URL path từ HTTP Request

+ Gửi nội dung file tương ứng với từng đường dẫn:

    - / → index.html

    - /styles.css → styles.css

    - /data → data.json hoặc data.csv

+ Xử lý 404 nếu path không tồn tại

# Bước 4 
Khi trình duyệt (hoặc app) gửi POST /save-data, server sẽ:

+ Nhận nội dung body gửi lên (JSON, text, form,...)

+ Ghi nội dung đó vào file (data.txt, data.json, v.v.)

+ Phản hồi lại “200 OK”


Quy trình:
    => chạy chương trình 
    => chạy lệnh trên cmd để gửi dữ liệu 
        bash : curl -X POST http://localhost:8080/save-data -H "Content-Type: application/json" -d "{\"temp\":27.5,\"voltage\":3.3}"

# Bước 5

+ Tạo mỗi client một thread riêng.

+ Giúp server có thể phục vụ nhiều trình duyệt hoặc máy cùng lúc.
# Bước 6 

+ Tạo một trang HTML có form nhập dữ liệu

+ Gửi dữ liệu từ form bằng POST

+ Server nhận, xử lý và lưu lại vào file như ở bước 4 và 5

+ Hiển thị thông báo Đã gửi thành công nếu mọi thứ ổn
<!-- 
    <form>	Gửi dữ liệu tới server

    method="POST"	Chỉ định kiểu gửi là POST

    action="/save-data"	Chỉ định URL sẽ nhận dữ liệu

    application/x-www-form-urlencoded	Dạng mặc định của dữ liệu form
 -->
 # Bước 7 
Hiển thị dữ liệu đã lưu ra trình duyệt bằng cách xử lý HTTP GET /data.

Khi truy cập http://localhost:8080/data, server sẽ:

+ Đọc nội dung file saved_data.json

+ Gửi nội dung đó về trình duyệt dưới dạng JSON hoặc HTML

+ Trình duyệt hiển thị dữ liệu người dùng đã gửi
<!-- 
    + fopen("r") + fread()	Đọc toàn bộ nội dung file
    
    + MIME type application/json	Giúp trình duyệt hiểu là JSON
    
    + Có thể trả về dạng HTML nếu muốn dễ xem hơn	

 -->