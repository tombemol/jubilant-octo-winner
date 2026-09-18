CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude
SRC := $(wildcard src/*.c)
BIN := build/livraria

ifeq ($(OS),Windows_NT)
BIN := build/livraria.exe
endif

all: $(BIN)

$(BIN): $(SRC) | build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

build:
	mkdir -p build

run: $(BIN)
	./$(BIN)

clean:
	rm -rf build

.PHONY: all run clean
