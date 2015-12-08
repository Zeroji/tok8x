CFLAGS+=-Wall -O2 $(shell pkg-config --cflags popt)
CFLAGSDEBUG=-Wall -Wunreachable-code -ggdb3 -O0 -DDEBUG $(shell pkg-config --cflags popt)
LDFLAGS+=-Wall -O2 $(shell pkg-config --libs popt)
LDFLAGSDEBUG=-Wall -Wunreachable-code -ggdb3 -O0 -DDEBUG $(shell pkg-config --libs popt)
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
