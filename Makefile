# Implementace interpretu imperativniho jazyka IFJ15
################################################################################
# Autori:       xvitas02 – Jakub Vitásek
# 				xvidaj00 - Juraj Vida
# 				
# Popis:		Makefile pro cely interpret
################################################################################

CFLAGS 	= -std=c99 -Wall -Wextra -pedantic
CC 		= gcc

main: main.c
	$(CC) $(CFLAGS) error.c interpret.c instlist.c scanner.c parser.c str.c main.c expression.c ial.c istack.c -o main

clean:
	rm main
