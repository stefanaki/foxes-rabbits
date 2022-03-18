.PHONY: all clean

CC = gcc
CFLAGS = -g -Wall -O2 -Iinclude
LIBS = 
SHELL = /bin/bash
OUT = ./out

all: $(OUT)/main

$(OUT)/main: $(OUT)/main.o $(OUT)/worldgen.o $(OUT)/cell.o $(OUT)/animal.o $(OUT)/serial.o
	$(CC) $(CFLAGS) $(LIBS) $(OUT)/main.o $(OUT)/worldgen.o $(OUT)/cell.o $(OUT)/animal.o $(OUT)/serial.o -o $@

$(OUT)/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OUT)/* $(OUT)/*/*
