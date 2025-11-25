TARGET = app
CC 	   = gcc

# Quy tắc linker và tạo ra file .exe 
build: $(TARGET)

$(TARGET):
	$(CC) main.c -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -rf $(TARGET)

.PHONY: run build 

