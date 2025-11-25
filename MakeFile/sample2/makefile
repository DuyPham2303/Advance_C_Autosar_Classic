# BASIC 
# CC = gcc
# CFLAGS = -Iinc -Wall -Werror -g

# app: main.o test1.o test2.o
# 	$(CC) main.o test1.o test2.o -o app.exe

# main.o: src/main.c
# 	$(CC) $(CFLAGS) -c src/main.c -o main.o

# test1.o: src/test1.c
# 	$(CC) $(CFLAGS) -c src/test1.c -o test1.o

# test2.o: src/test2.c
# 	$(CC) $(CFLAGS) -c src/test2.c -o test2.o

# CÁCH TỔNG QUÁT - NÂNG CAO

# tên của tệp thực thi
TARGET = app

# Thư mục chứa header và source files
HEADER_DIR = inc
SOURCE_DIR = src

# Đọc ra các file nguồn và object tương ứng 
SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)

# Khai báo compiler và các cờ cần thiết
CC = gcc
CFLAGS = -I$(HEADER_DIR) -Wall -Werror -g

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ 

# Quy tắc để biên dịch các tệp .c -> .o
$(SOURCE_DIR)/%.o : $(SOURCE_DIR)/%.c 
	$(CC) $(CFLAGS) -c -o $@ $<

# Quy tắc để tạo ra tệp .exe
build: $(TARGET)

# Quy tắc để run
run:
	./$(TARGET)

clean:
	rm -rf $(SOURCE_DIR)/*.o $(TARGET)

.PHONY: all clean run build
