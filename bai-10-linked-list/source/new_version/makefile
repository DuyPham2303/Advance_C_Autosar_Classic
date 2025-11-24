TARGET = main.exe
CC = gcc

# Read all .c files in this directory
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

CFLAGS = -I. -Wall -Werror

.PHONY: all clean run

# Default target
all: $(TARGET)

# Link objects into executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) 
	