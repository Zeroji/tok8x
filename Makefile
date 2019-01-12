CFLAGS+=-Wall -D_XOPEN_SOURCE=700 -O2 $(shell pkg-config --cflags popt)
CFLAGSDEBUG=-Wall -D_XOPEN_SOURCE=700 -ggdb3 -O0 -DDEBUG $(shell pkg-config --cflags popt)
LDFLAGS+=-Wall -D_XOPEN_SOURCE=700 -O2 $(shell pkg-config --libs popt)
LDFLAGSDEBUG=-Wall -D_XOPEN_SOURCE=700 -ggdb3 -O0 -DDEBUG $(shell pkg-config --libs popt)
SRCDIR=./src
OBJDIR=./obj
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRC))
BIN=tok8x

all: $(OBJ)
	$(CC) -o $(BIN) $^ $(LDFLAGS)

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
