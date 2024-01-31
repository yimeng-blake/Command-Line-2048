.PHONY: clean
CC=gcc
CFLAGS=-g -Wall

all: game

game: game.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f game 