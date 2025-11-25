build: src/main.c
	cc src/main.c -o bin/main -Wall -g -lncurses

run: bin/main
	./bin/main
