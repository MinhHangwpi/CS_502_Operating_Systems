CC = gcc

CFLAGS = -Wall -g

TARGET = ../proj6

SRCS = main.c argparse.c thread.c readmode.c mmapmode.c 

all: clean $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
