CC=gcc
CFLAGS=-I./include -std=c99
SRC=terrain.c ent.c room.c
OUT=rogue_test
LINK=-lncurses

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LINK)
clean:
	rm $(OUT)
