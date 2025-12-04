# Tên của chương trình
TARGET = run

# Thư mục chứa header và source files
HEADER_DIR = Inc
SOURCE_DIR = Src
BINARY_DIR = bin

# Các tệp nguồn và tệp object tương ứng
SOURCES = $(wildcard *.c) $(wildcard $(SOURCE_DIR)/*.c) 
OBJECTS = $(patsubst %.c,$(BINARY_DIR)/%.o,$(notdir $(SOURCES)))
# Trình biên dịch và các cờ
CC = gcc
CFLAGS = -g -I$(HEADER_DIR) -Wall -Wextra -std=c17

# Quy tắc mặc định (biên dịch tự động ra tệp thực thi)
all: $(BINARY_DIR)/$(TARGET)

# Tạo thư mục bin nếu chưa có
$(BINARY_DIR): 
	mkdir -p $(BINARY_DIR)

# Link các object thành chương trình
$(BINARY_DIR)/$(TARGET): $(OBJECTS) | $(BINARY_DIR) 
	@echo "Linking..."
	$(CC) $(CFLAGS) -o $@ $^

# Build từng .c thành .o
$(BINARY_DIR)/%.o: %.c | $(BINARY_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINARY_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BINARY_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

# Chạy chương trình
run: $(BINARY_DIR)/$(TARGET)
	./$(BINARY_DIR)/$(TARGET)

# Quy tắc để làm sạch thư mục build
clean:
	rm -f $(BINARY_DIR)/*.o $(BINARY_DIR)/$(TARGET)

# Quy tắc giả (phony target)
.PHONY: all clean run





