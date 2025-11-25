# Lưu trữ thông tin các file
TARGET = app
HEADER_DIR = include 
SOURCE_DIR = src/app src/log src/math
BUILD_DIR = build

# Trích xuất ra các file .c ở trong các folder 
vpath %.c $(SOURCE_DIR)

# Lưu trữ các file sau khi trích xuất -> cần giải thích rõ 
SOURCES = $(foreach dir,$(SOURCE_DIR),$(wildcard $(dir)/*.c))

# Lưu trữ các file object -> cần giải thích rõ 
SOURCE_NAMES = $(notdir $(SOURCES))
OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCE_NAMES))

# Khai báo compiler và flags
CC = gcc 
CFLAGS = -I$(HEADER_DIR) -Wall -Werror -g

# Quy tắc để build ra .exe
all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Quy tắc để liên kết các tệp .o -> .exe
# tại sao cần phải có | $(BUILD_DIR)  ? -> giải thích
$(BUILD_DIR)/$(TARGET): $(OBJECTS) | $(BUILD_DIR) 
	@echo "Linking $@"
	$(CC) -o $@ $^ 

# Quy tắc để build .c -> .o
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

# Quy tắc để run  .exe
run: 
	./$(BUILD_DIR)/$(TARGET)

# Quy tắc để clean
clean: 
	rm -rf $(BUILD_DIR)

.PHONY: build run clean