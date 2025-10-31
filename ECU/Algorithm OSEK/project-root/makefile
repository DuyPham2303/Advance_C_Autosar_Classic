CC       = cc
CFLAGS   = -std=c11 -O2 -Wall -Wextra
INCLUDES = -Iinclude

BUILD_DIR = bin
OBJ_DIR   = $(BUILD_DIR)/obj

SRC  = src/sha256.c src/ecc_toy.c src/main.c
OBJ  = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))
TARGET = $(BUILD_DIR)/ecdsa_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

# tiện: chạy nhanh
run: $(TARGET)
	$(TARGET)
