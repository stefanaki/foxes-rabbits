.PHONY: all clean

CC = mpicc
CFLAGS = -g -Wall -O3 -Iinclude

OUT = out
SRC = src

SOURCES = $(filter-out $(SRC)/serial-implementation.c $(SRC)/openmp-implementation.c, $(wildcard $(SRC)/*.c))
OBJECTS = $(patsubst $(SRC)/%.c, $(OUT)/%.o, $(SOURCES))

all: foxes-rabbits

foxes-rabbits: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(OUT)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUT)/* $(OUT)/*/* foxes-rabbits
