#ifndef _MATCH_H_
#define _MATCH_H_

#include "board.h"
#include "types.h"
#include "units.h"

#include <stdlib.h>

typedef enum direction
{
  UP,
  DOWN,
  LEFT,
  RIGHT
} direction;

typedef struct Match
{
  board *board1;
  board *board2;

  hero *hero1;
  hero *hero2;

  point cursorPosition;
  bool hasUnitSelected;

  point selectedPosition;
  unit selectedUnit;

  // TODO note debug var
  bool debug;
} match;

match *newMatch (hero *hero1, hero *hero2);

void freeMatch (match *m);

void runMatch (match *match);
void handleInput (match *m);

void moveCursor (match *m, direction d);
void deleteUnit (match *m);

#endif