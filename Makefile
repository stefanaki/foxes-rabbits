.PHONY: all clean

CC = gcc
CFLAGS = -g -Wall -O3 -Iinclude -fopenmp

OUT = out
SRC = src

SOURCES = $(filter-out $(SRC)/serial-implementation.c, $(wildcard $(SRC)/*.c))
OBJECTS = $(patsubst $(SRC)/%.c, $(OUT)/%.o, $(SOURCES))

all: foxes-rabbits

foxes-rabbits: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(OUT)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUT)/* $(OUT)/*/* foxes-rabbits
