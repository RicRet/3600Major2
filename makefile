CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm
OBJ = shellmain.o commands.o
EXEC = major2shell

# Default target to compile everything
all: $(EXEC)

# Rule to compile the main shell executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Rule to compile the shellmain.o object file
shellmain.o: shellmain.c shellmain.h
	$(CC) $(CFLAGS) -c shellmain.c

# Rule to compile the commands.o object file (assuming you have commands.c)
commands.o: commands.c commands.h
	$(CC) $(CFLAGS) -c commands.c

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(EXEC)

# Rule to clean and rebuild everything
rebuild: clean all
