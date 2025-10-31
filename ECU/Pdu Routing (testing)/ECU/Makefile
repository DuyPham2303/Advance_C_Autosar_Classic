# Compiler and flags
CC = gcc
LIBS = -pthread -lws2_32
CFLAGS = -Wall -g\
-I.\BSW\MCAL\
-I.\BSW\Services\
-I.\RTE\
-I.\SWC
# Object directory
OBJDIR = bin
# Executable
TARGET = $(OBJDIR)/ecu

SRC = .\BSW\MCAL\Can.c \
.\BSW\Services\CanIf.c \
.\BSW\Services\Com.c \
.\BSW\Services\csv_lib.c \
.\BSW\Services\PduR.c \
.\main.c \
.\RTE\Rte_SpeedControl.c \
.\RTE\Rte_StopEngine.c \
.\SWC\SpeedControl.c \
.\SWC\StopEngine.c 

# Object files
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

# Build executable
$(TARGET): $(OBJ)
	@echo "Linking all objects to create $(TARGET)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS)
	@echo "Build successful!"

# Compile object files
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $<"

# Clean up
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJDIR)
	@echo "Clean done!"

# Run program
.PHONY: run
run: $(TARGET)
	@echo "Running program..."
	$(TARGET)
