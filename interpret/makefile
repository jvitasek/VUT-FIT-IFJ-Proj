CFLAGS=-std=c99 -Wall -pedantic -g
BIN=project
CC=gcc
RM=rm -f

ALL: ilist.o interpret.o stack.o
	$(CC) $(CFLAGS) -o $(BIN) interpret.o ilist.o stack.o

clean:
	$(RM) *.o $(BIN)
