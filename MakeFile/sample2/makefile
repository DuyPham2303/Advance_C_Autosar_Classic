# BASIC 
CC = gcc
CFLAGS = -Iinc -Wall -Werror -g 

app: main.o test1.o test2.o
	$(CC) main.o test1.o test2.o -o app
main.o: src/main.c 
	$(CC) $(CFLAGS) -c src/main.c -o main.o

test1.o: src/test1.c inc/test1.h
	$(CC) $(CFLAGS) -c src/test1.c -o test1.o

test2.o: src/test2.c inc/test2.h
	$(CC) $(CFLAGS) -c src/test2.c -o test2.o

clean:
	rm -f *.o app

.PHONY: clean app

# CÁCH TỔNG QUÁT 

# Tên của chương trình
TARGET = app

# Thư mục chứa header và source files
HEADER_DIR = inc
SOURCE_DIR = src

# Các tệp nguồn và tệp object tương ứng
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS := $(SOURCES:.c=.o)

# Trình biên dịch và các cờ
CC = gcc
CFLAGS = -I$(HEADER_DIR) -Wall -Wextra

# Quy tắc để tạo ra tệp thực thi
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

# Quy tắc để biên dịch các tệp .c thành .o
$(SOURCE_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Quy tắc để build file thực thi
build: $(TARGET)

# quy tắc để build & run
run: 
	./$(TARGET)

# Quy tắc để làm sạch thư mục build
clean:
	rm -f $(SOURCE_DIR)/*.o $(TARGET)

# Quy tắc giả (phony target)
.PHONY: all clean run build




