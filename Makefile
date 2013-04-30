CC=gcc
CFLAGS=-Wall

SRC=tokens.c \
	parse.c \
	tokenise.c \
	detokenise.c \
	tok8x.c
OBJ=$(patsubst %.c,%.o,$(SRC))
BIN=tok8x

TESTFILE=out.8xp

all: compile build
	
compile:
	$(CC) $(CFLAGS) -c $(SRC)
build: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)
clean:
	rm -f $(OBJ) $(BIN) $(TESTFILE)
