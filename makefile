CC      = gcc
CFLAGS  = -g
RM      = rm -f

default: all

all: stratgame

stratgame: main.c
	   $(CC) main.c $(CFLAGS) -o build/game

clean veryclean:
	$(RM) build/game
