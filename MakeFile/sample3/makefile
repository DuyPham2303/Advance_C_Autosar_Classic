# ======== CONFIG ========
TARGET      = app
INC_DIR     = include
SRC_ROOT    = src/app src/log src/math
BUILD_DIR   = build

# Search .c in all source directories
vpath %.c $(SRC_ROOT)

# List all .c files
SOURCES     = $(foreach dir,$(SRC_ROOT),$(wildcard $(dir)/*.c))

# Extract file names only, convert to .o inside build/
SRC_NAMES   = $(notdir $(SOURCES))
OBJECTS 	= $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_NAMES))


# Compiler flags
CC          = gcc
CFLAGS      = -g -Wall -Wextra -I$(INC_DIR)

# ======== DEFAULT ========
all: $(BUILD_DIR)/$(TARGET)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# ======== LINKING ========
$(BUILD_DIR)/$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	@echo ">> Linking $@"
	$(CC) -o $@ $^

# ======== COMPILE EACH FILE ========
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo ">> Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

# ======== RUN ========
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

# ======== CLEAN ========
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
