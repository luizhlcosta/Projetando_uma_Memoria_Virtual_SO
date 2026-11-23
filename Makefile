CC = gcc
CFLAGS = -Wall -Wextra
TARGET = vm
OBJS = vm.o memoria.o 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

vm.o: vm.c
	$(CC) $(CFLAGS) -c vm.c

memoria.o: memoria.c memoria.h
	$(CC) $(CFLAGS) -c memoria.c 
	
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean