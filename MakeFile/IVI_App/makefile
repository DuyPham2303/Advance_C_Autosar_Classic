# Tên chương trình và thư mục
TARGET = run
HEADER_DIR = Inc
SOURCE_DIRS = Src demo
BINARY_DIR = bin

# Danh sách source và object files (vpath cho phép tìm .c trong nhiều folder)
vpath %.c $(SOURCE_DIRS)
SOURCES = $(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.c))
OBJECTS = $(patsubst %.c,$(BINARY_DIR)/%.o,$(notdir $(SOURCES))) 

# Trình biên dịch và flags
CC = gcc
CFLAGS = -g -I$(HEADER_DIR) -Wall -Wextra

all: $(BINARY_DIR)/$(TARGET)

# Tạo thư mục bin nếu chưa có
$(BINARY_DIR):
	mkdir -p $(BINARY_DIR)

# Link các object thành file thực thi trong bin/
$(BINARY_DIR)/$(TARGET): $(OBJECTS) | $(BINARY_DIR)
	@echo "Linking $@"
	$(CC) -g -o $@ $^

# Quy tắc biên dịch mỗi file .c thành file .o trong bin/
$(BINARY_DIR)/%.o: %.c | $(BINARY_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Quy tắc mặc định
run: $(BINARY_DIR)/$(TARGET)
	./$(BINARY_DIR)/$(TARGET)

# Quy tắc dọn sạch
clean:
	rm -rf $(BINARY_DIR) $(TARGET)

# Quy tắc giả
.PHONY: all clean run
