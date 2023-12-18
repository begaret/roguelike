CC=cc
CFLAGS=-Wall -Wextra -Ofast

SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c, int/%.o, $(SRC))

# TODO: dynamic linking depending on backend
LIB=-lallegro -lallegro_main -lallegro_image -lncurses

OUT=bin/main

.PHONY: all clean

int/%.o: src/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $^ -o $@ $(LIB) # -fsanitize=address

clean:
	rm -f int/*

