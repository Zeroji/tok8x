CC=gcc
CFLAGS=-Wall
LDFLAGS=-Wall
SRCDIR=./src
OBJDIR=./obj
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRC))
BIN=tok8x

all: $(OBJ)
	$(CC) $(LDFLAGS) -o $(BIN) $^

$(OBJ): | $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(BIN)

new: clean all
