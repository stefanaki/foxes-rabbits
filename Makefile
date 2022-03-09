.PHONY: all clean

CC = gcc
CFLAGS = -g -Wall -O2 -Iinclude
LIBS = 
SHELL = /bin/bash
OUT = ./out

all: main

main: main.o worldgen.o
	$(CC) $(CFLAGS) $(LIBS) main.o worldgen.o -o $(OUT)/main

%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $<

%.o: ./lib/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main worldgen