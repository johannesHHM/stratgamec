#ifndef _BOARD_H_
#define _BOARD_H_

#include "heros.h"

#include <stdbool.h>

typedef struct Board
{
  int WIDTH;
  int HIGHT;
  unit *board[6][8];
  int backupUnits;
} board;

board *newBoard ();
bool sendUnit (board *b, unit *u, int y);
bool removeUnit (board *b, int x, int y);
bool sendBackupUnits (board *b, int amount, hero *h);

bool checkTagsNeeded (board *b);
bool checkAttack3x1 (board *b);
bool checkWall (board *b);

void tagAttacks3x1 (board *b);
void tagWalls (board *b);

void makeWalls (board *b, hero *h);

void printBoard (board *b);

#endif