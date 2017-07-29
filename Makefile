CC=gcc
CFLAGS=-I./include -std=c99 -g
SRC=terrain.c ent.c room.c
OUT=rogue_test
LINK=-lncursesw

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LINK)
clean:
	rm $(OUT)
