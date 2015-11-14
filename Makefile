main: main.c
	gcc -std=c99 -Wall -Wextra -pedantic scanner.c parser.c str.c main.c -o main

clean:
	rm main
