# Makefile for compiling a program with main.c, mailbox.c, and mailbox.h, queue.c, and queue.h

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Target executable name
TARGET = mailbox

# Source files
SRCS = main.c mailbox.c queue.c

# Default target
all: clean $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Clean target for cleaning up
clean:
	rm -f $(TARGET)

.PHONY: all clean
