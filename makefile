CC = gcc

IDIR = include
SDIR = src
FDIR = files

CFLAGS = -std=c99 -I $(IDIR) -Wall
LIBS = -lncurses

ENAME = ctetris

# Target to check and create the directory if it doesn't exist
check_dir:
	@[ -d $(FDIR) ] || mkdir -p $(FDIR)

_DEPS = game.h menu.h scoreboard.h info.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_SRC = main.c game.c menu.c scoreboard.c info.c
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

all: $(SRC) $(DEPS)
	$(CC) -o $(ENAME) $(SRC) $(CFLAGS) $(LIBS)

.PHONY: clean

