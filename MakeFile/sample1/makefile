TARGET = main
CC = gcc

main.i: 
	gcc -E main.c -o main.i
main.s: 
	gcc -S main.i -o main.s
main.o: 
	gcc -c main.s -o main.o

build: $(TARGET)

$(TARGET): 
	$(CC) main.c -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -f *.o *.i *.s, $(TARGET)

.PHONY: run clean build phase1 phase2 phase3

