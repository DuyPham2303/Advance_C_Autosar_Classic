# Tên chương trình và thư mục
TARGET = run
HEADER_DIR = Inc
SOURCE_DIR = Src
BUILD_DIR = bin

# Danh sách source và object files
SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Trình biên dịch và flags
CC = gcc
CFLAGS = -g -I$(HEADER_DIR) -Wall -Wextra

# Tạo thư mục bin nếu chưa có
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Quy tắc biên dịch mỗi file .c thành file .o trong bin/
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Link các object thành file thực thi trong bin/
$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@echo "Linking $@"
	$(CC) -o $@ $^

# Quy tắc mặc định
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/*.exe

# Quy tắc dọn sạch
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Quy tắc giả
.PHONY: all clean
