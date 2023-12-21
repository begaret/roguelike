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

APP=bin/roguelike-v0.0.0
OUT=bin/main

.PHONY: all dirs clean

int/%.o: src/%.c
	$(CC) -c $^ -o $@ $(INC) $(CFLAGS)

all: dirs $(APP)

dirs:
ifeq ($(wildcard bin/*),)
	mkdir -p bin/
endif
ifeq ($(wildcard int/*),)
	mkdir -p int/
endif
ifeq ($(wildcard data/*),)
	mkdir -p data/
endif

$(APP): $(OUT)
	mkdir -p $(APP)/
	find data | cpio -pdm $(APP)/
	cp $(OUT) $(APP)/
	zip -qr $(APP){.zip,}

$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIB) # -fsanitize=address

clean:
	rm -f int/*
	rm -rf $(APP)/

