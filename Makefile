.PHONY: all clean

CC = gcc
CFLAGS = -g -Wall -O3 -Iinclude -fopenmp
LIBS = 
SHELL = /bin/bash
OUT = ./out

all: $(OUT)/foxes-rabbits

$(OUT)/foxes-rabbits: $(OUT)/foxes-rabbits.o $(OUT)/worldgen.o $(OUT)/cell.o $(OUT)/animal.o $(OUT)/serial.o
	$(CC) $(CFLAGS) $(LIBS) $(OUT)/foxes-rabbits.o $(OUT)/worldgen.o $(OUT)/cell.o $(OUT)/animal.o $(OUT)/serial.o -o $@

$(OUT)/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OUT)/* $(OUT)/*/*
