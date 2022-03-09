.PHONY: all clean

CC = gcc
CFLAGS = -g -Wall -O2 -Iinclude
LIBS = 
SHELL = /bin/bash
OUT = ./out

all: main

main: $(OUT)/main.o $(OUT)/worldgen.o
	$(CC) $(CFLAGS) $(LIBS) $(OUT)/main.o $(OUT)/worldgen.o -o $(OUT)/main

$(OUT)/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o main worldgen