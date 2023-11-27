#ifndef _MATCH_H_
#define _MATCH_H_

#include "board.h"
#include "types.h"
#include "units.h"

#define KEY_DIRECTION_START 262
#define HOLD_COOLDOWN 12
#define HOLD_PAUSE 6

typedef enum direction
{
  RIGHT = 0,
  LEFT = 1,
  DOWN = 2,
  UP = 3
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

  bool movingDirection[4];
  int cooldownCounter[4];
  int pauseCounter[4];

  // TODO note debug var
  bool debug;
} match;

match *newMatch (hero *hero1, hero *hero2);

void freeMatch (match *m);

void runMatch (match *match);

void handleInput (match *m);
void handleInputLocked (match *m);

void updateHoldState (match *m);

bool canMoveCursor (match *m, direction d);
void moveCursor (match *m, direction d);
void setCursorTop (match *m);

void deleteUnit (match *m);

#endif
