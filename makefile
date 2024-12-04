#makefile for major2, will create an executable called shell
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm
TARGET = major2shell
SOURCES = main.c shellmain.c commands.c
OBJECTS = $(SOURCES:.c=.o)

# Default rule
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
