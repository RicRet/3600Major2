#makefile for major 2, will create executable named major2shell
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm
OBJ = shellmain.o commands.o
EXEC = major2shell

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

shellmain.o: shellmain.c shellmain.h
	$(CC) $(CFLAGS) -c shellmain.c

commands.o: commands.c commands.h
	$(CC) $(CFLAGS) -c commands.c

clean:
	rm -f $(OBJ) $(EXEC)

rebuild: clean all
