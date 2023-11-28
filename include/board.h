#ifndef _BOARD_H_
#define _BOARD_H_

#include "heros.h"
#include "types.h"

#include <stdbool.h>

#define WIDTH_C 6
#define HIGHT_C 8

// TODO maybe make board width & hight custom,
// aka make board unit* and malloc size from
// WIDTH and HIGHT
// TODO change backup units to hero ?
typedef struct Board
{
  int WIDTH;
  int HIGHT;
  unit board[WIDTH_C][HIGHT_C];
  int backupUnits;
} board;

board *newBoard (int maxUnits);
void freeBoard (board *b);

bool sendUnit (board *b, unit u, int y);
bool sendUnitBackup (board *b, unit u, int y);
bool removeUnit (board *b, int x, int y);
bool sendBackupUnits (board *b, hero *h);

int getTopFree (board *b, int y);
point getTopUnit (board *b, int y);
bool moveUnitToColumn (board *b, point p0, int y1);
bool moveUnit (board *b, int x0, int y0, int x1, int y1);

bool checkTagsNeeded (board *b);
bool checkAttack3x1 (board *b);
bool checkWall (board *b);

bool checkFullBoard (board *b);
bool checkLockedAnimations (board *b);

void tagAttacks3x1 (board *b);
void tagWalls (board *b);

bool makeWalls (board *b, hero *h);

bool sinkWalls (board *b);
bool sinkAttacks3x1 (board *b);
bool sinkUnits (board *b);

bool makeAttacks3x1 (board *b, hero *h);

void updateWalkingUnit (board *b, int x, int y, int direction);

void printBoard (board *b);

#endif
