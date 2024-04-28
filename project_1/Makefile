# Define compiler and flags
CC=gcc
CFLAGS=-c

# Define directories
OBJDIR=obj

# Define source and object files
SOURCES=main.c stack.c
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_WITH_DIR=$(addprefix $(OBJDIR)/, $(OBJECTS))

# Define the target executable
TARGET=doit

# Default target
all: $(TARGET)

$(TARGET): $(OBJDIR) $(OBJECTS_WITH_DIR)
	$(CC) $(OBJECTS_WITH_DIR) -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Phony targets
.PHONY: all clean
