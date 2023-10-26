all: main

main: main.c execute.c
	gcc -std=gnu17 -Wvla -Wall -Wextra -Werror -Wpedantic -Wno-unused-result -Wconversion main.c execute.c signals.c -o main 

clean: rm -f main