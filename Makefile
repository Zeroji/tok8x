CC=gcc
CFLAGS=-Wall -O2
CFLAGSDEBUG=-Wall -Wunreachable-code -ggdb3 -O0 -DDEBUG
LDFLAGS=-Wall -O2
LDFLAGSDEBUG=-Wall -Wunreachable-code -ggdb3 -O0 -DDEBUG
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

debug: CFLAGS=$(CFLAGSDEBUG)
debug: LDFLAGS=$(LDFLAGSDEBUG)
debug: all

new: clean all
