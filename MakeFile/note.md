# Dẫn dắt
    - khi build bình thường bằng lệnh gcc -> tốn tgian,dễ lỗi
# Giới thiệu
- tệp văn bản (ko phải tool), chứa các chỉ thị cho phép build program tự động,nhanh chóng
- chứa các 
    - biến
    - quy tắc 
    - Lệnh
- chạy bằng lệnh make -> tool để chạy tệp makefile
- Cho phép biên dịch,liên kết tự động nhiều phần của 1 project  
    -> hạn chế lỗi do biên dịch thủ công
    -> tự động đọc file và chỉ biên dịch các phần thay đổi trong chương trình => tránh biên dịch lại toàn bộ dự án
# Tạo makefile
    - 3 cách tạo file
        + Makefile
        + makefile
        + file.mk
# Thành phần makefile
    - rules (quy tắc) ? -> chứa lệnh muốn thực thi 
    - command (lệnh) -> Thay thế vào lệnh muốn chạy
    - dependency -> file phụ thuộc (có thể có / không)
    ví dụ : target: dependency
                commands 
    => ý nghĩa các phần trên ?
    ví dụ : chạy compiler 
        -> triển khai các rule (phase 1 -> 4, run:)
        -> triển khai rule xóa file
# Biến trong makefile 
## so sánh = và :=

- Khi biến phụ thuộc wildcard → dùng := (đánh giá trước -> chạy sau)

CC = gcc
CFLAGS = -Wall

- Các biến cấu hình compiler → dùng = (đánh giá sau -> chạy trước)

SRC := $(wildcard src/*.c)


# Automatic variable ($@ , $^ , $< , ...)

//$@  : tên target
//$<  : dependency đầu tiên (khi .c -> .o)
//$^  : toàn bộ dependencies (khi link .o để tạo .exe)


# Pattern rules 

## tự động hóa 
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

=> Bất cứ file .o nào phụ thuộc vào 1 file .c cùng tên, sẽ được compile theo câu lệnh này 

## pattern rules với directory

``````````
src/main.c → build/main.o
``````````

``````````
OBJ := $(patsubst src/%.c, build/%.o, $(SRC))

build/%.o: src/%.c
    $(CC) $(CFLAGS) -c $< -o $@
``````````

# Các hàm hỗ trợ 

## wildcard : 
$(wildcard pattern ...) - tìm và lấy các file trong thư mục chỉ định

`ví dụ`
SRC := $(wildcard src/*.c)

`kết quả`
SRC = src/main.c src/utils.c src/driver.c

## Substitution reference 
$(LIST:old=new) - Thay thế đuôi file

`ví dụ`
SRC := src/main.c src/utils.c
OBJ := $(SRC:.c=.o)

`kết quả` 
OBJ = src/main.o src/utils.o

## subst
$(subst from,to,text) 
- thay thế chuỗi đơn giản 
- khi thay thế chuỗi 1-1 , không phải theo file-pattern

`ví dụ`
STR := "Hello_World_C_Code"
NEW := $(subst , _,$(STR))

`kết quả `
NEW = "Hello World C Code"

## patsubst
$(patsubst pattern,replacement,text)
- Thay thế theo pattern (%)
- cho phép hiểu và xử lý được wilcard % để match pattern

`ví dụ`
SRC := src/main.c src/utils.c

OBJ := $(patsubst src/%.c, build/%.o, $(SRC))

kết quả
OBJ = build/main.o build/utils.o


