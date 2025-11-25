# Tên chương trình và thư mục
TARGET = run
HEADER_DIR = Inc
SOURCE_DIRS = Src
BINARY_DIR = bin

# Danh sách source và object files
SOURCES = $(wildcard $(SOURCE_DIRS)/*.c)

# Chỉ lấy tên file (không kèm đường dẫn)
SOURCE_NAMES = $(notdir $(SOURCES))

# object nằm trong bin/ nhưng vẫn giữ đúng tên
OBJECTS = $(patsubst %.c,$(BINARY_DIR)/%.o,$(SOURCE_NAMES))

# Trình biên dịch và flags
CC = gcc
CFLAGS = -g -I$(HEADER_DIR) -Wall -Wextra

all: $(BINARY_DIR)/$(TARGET)

# Tạo thư mục bin nếu chưa có
$(BINARY_DIR):
	mkdir -p $(BINARY_DIR)

# Link
$(BINARY_DIR)/$(TARGET): $(OBJECTS) | $(BINARY_DIR)
	@echo "Linking $@"
	$(CC) -g -o $@ $^

# Quy tắc build .c -> .o
# Dùng vpath để tìm file .c từ SOURCE_DIRS
vpath %.c $(SOURCE_DIRS)

$(BINARY_DIR)/%.o: %.c | $(BINARY_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(BINARY_DIR)/$(TARGET)
	./$(BINARY_DIR)/$(TARGET)

clean:
	rm -rf $(BINARY_DIR)

.PHONY: all clean run
