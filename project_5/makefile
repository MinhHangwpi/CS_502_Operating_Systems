CC = gcc

CFLAGS = -Wall -g

TARGET = maze

SRCS = main.c room.c rat.c stack.c

all: clean $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
