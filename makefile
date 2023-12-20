UNAME=$(shell uname)

CC=cc
CFLAGS=-Wall -Wextra -Ofast

# SRC=$(wildcard src/*.c)
SRC=$(filter-out  src/al_term.c, $(wildcard src/*.c))
OBJ=$(patsubst src/%.c, int/%.o, $(SRC))

ifeq ($(UNAME), Darwin)
LIB=\
	-lSDL2 -lGLEW -framework OpenGL\
		-lncurses
else
LIB=\
	-lm\
	-lSDL2 -lGLEW -lGL\
		-lncurses
endif
INC=-Iinclude/

RES=colors.txt curses.bmp
APP=bin/roguelike-v0.0.0
OUT=bin/main

.PHONY: all clean

int/%.o: src/%.c
	$(CC) -c $^ -o $@ $(INC) $(CFLAGS)

all: $(APP)
ifeq ($(wildcard bin/*),)
	mkdir bin/
endif
ifeq ($(wildcard int/*),)
	mkdir int/
endif


$(APP): $(OUT)
	mkdir -p $(APP)/
	cp $(OUT) $(RES) $(APP)/

$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIB) # -fsanitize=address

clean:
	rm -f int/*
	rm -rf $(APP)/

