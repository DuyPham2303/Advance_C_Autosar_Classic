# Compiler and flags
CC       := gcc
CFLAGS   := -std=c99 -Wall -Wextra -pthread

# Output directory for object files and binary
BIN_DIR    := bin

# Target binary name (inside BIN_DIR)
BINARY     := $(BIN_DIR)/can_app

# Source files
SRCS := main.c                  \
        csv_lib.c               \
        Can.c                   \
        CanIf.c                 \
        PduR.c                  \
        Com.c                   \
        Rte_SpeedControl.c      \
        Rte_StopEngine.c        \
        SpeedControl.c          \
        StopEngine.c

# Object files will be placed under BIN_DIR, mirroring the source filenames
OBJS := $(SRCS:%.c=$(BIN_DIR)/%.o)

# Default target: build the binary
all: $(BINARY)

# Link the final executable into BIN_DIR
$(BINARY): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile each .c into a corresponding .o under BIN_DIR
# Ensures BIN_DIR exists before compiling
$(BIN_DIR)/%.o: %.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule: remove bin directory entirely
clean:
	rm -rf $(BIN_DIR)

# run: first clean, then rebuild, then execute
run: clean all
	@./$(BINARY)

.PHONY: all clean run
