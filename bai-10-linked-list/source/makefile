TARGET = main
CC = gcc

SOURCES = $(wildcard *.c)
CFLAGS = -fdiagnostics-color=always -std=c17 -Wall -Werror -g
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

# build executable
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

# build .o from .c
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run:
	./$(TARGET) 

clean:
	rm -f $(OBJECTS) $(TARGET) 

.PHONY: all clean run
