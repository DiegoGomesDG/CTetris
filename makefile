CC = gcc

IDIR = include
SDIR = src

CFLAGS = -std=c99 -I $(IDIR) -Wall
LIBS = -lncurses

ENAME = ctetris

_DEPS = game.h menu.h leaderboard.h info.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_SRC = main.c game.c menu.c leaderboard.c info.c
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

all: $(SRC) $(DEPS)
	$(CC) -o $(ENAME) $(SRC) $(CFLAGS) $(LIBS)

.PHONY: clean

