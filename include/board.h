#ifndef _BOARD_H_
#define _BOARD_H_

#include "heros.h"

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
bool removeUnit (board *b, int x, int y);
bool sendBackupUnits (board *b, int amount, hero *h);

bool checkTagsNeeded (board *b);
bool checkAttack3x1 (board *b);
bool checkWall (board *b);

void tagAttacks3x1 (board *b);
void tagWalls (board *b);

void makeWalls (board *b, hero *h);

void sinkWalls (board *b);
void sinkAttacks3x1 (board *b);

void makeAttacks3x1 (board *b, hero *h);

void printBoard (board *b);

#endif