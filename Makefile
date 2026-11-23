CC = gcc
CFLAGS = -Wall -Wextra
TARGET = vm
OBJS = vm.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

vm.o: vm.c
	$(CC) $(CFLAGS) -c vm.c

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean