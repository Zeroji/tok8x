CC=gcc
CFLAGS=-Wall

SRC=tok8x.c \
	tokens.c
OBJ=$(patsubst %.c,%.o,$(SRC))
BIN=tok8x

all: compile build
	
compile:
	gcc -Wall -c $(SRC)
build:
	gcc -Wall -o $(BIN) $(OBJ)
clean:
	rm -f $(OBJ) $(BIN)
