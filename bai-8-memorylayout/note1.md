QUY trình xử lý của file thực thi sau khi load lên 

# trên PC
- Os loader đọc file thực thi -> map các segment (code,data,stack,heap) vào RAM
- Os ánh xạ virtual address (section header table) thông qua MMU (MEMORY MANAGEMENT UNIT) sang physic address
- Program counter bắt đầu thực thi main
# trên MCU
- CPU truy cập trực tiếp .text và .rdata từ flash (ko copy xuống RAM)
- Startup code tiến hành copy .data từ flash xuống RAM và zero hóa .bss 
