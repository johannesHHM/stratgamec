#include "board.h"
#include "heros.h"
#include "units.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

board *
newBoard ()
{
  board *b = (board *)malloc (sizeof (board));
  b->HIGHT = 6;
  b->WIDTH = 8;
  for (int x = 0; x < b->HIGHT; ++x)
    {
      for (int y = 0; y < b->WIDTH; ++y)
        {
          b->board[x][y] = NULL;
        }
    }
  b->backupUnits = 30;
  return b;
}

void
printBoard (board *b)
{
  printf ("              Board\n");
  for (int x = 0; x < b->HIGHT; ++x)
    {
      printf ("\n");
      for (int y = 0; y < b->WIDTH; ++y)
        {
          if (b->board[x][y])
            {
              printf (" %c%d ", b->board[x][y]->icon, b->board[x][y]->color);
            }
          else
            {
              printf (" __ ");
            }
        }
    }
  printf ("\n\n");
}

bool
sendUnit (board *b, unit *u, int y)
{
  if (b->board[b->HIGHT - 1][y] != NULL)
    {
      return false;
    }
  int x = b->HIGHT - 1;
  while (x >= 0 && b->board[x][y] == NULL)
    {
      x--;
    }
  b->board[x + 1][y] = u;
  if (checkTagsNeeded (b))
    {
      freeUnit (u);
      b->board[x + 1][y] = NULL;
      return false;
    }
  return true;
}

bool
removeUnit (board *b, int x, int y)
{
  if (!b->board[x][y])
    return false;
  freeUnit (b->board[x][y]);
  b->board[x][y] = NULL;
  return true;
}

void
tagWalls (board *b)
{
  for (int x = 0; x < b->HIGHT; ++x)
    {
      for (int y = 0; y < b->WIDTH - 2; ++y)
        {
          if (!b->board[x][y])
            continue;
          if (cmpUnits (b->board[x][y], b->board[x][y + 1])
              && cmpUnits (b->board[x][y], b->board[x][y + 2]))
            {
              b->board[x][y]->taggedWall = true;
              b->board[x][y]->icon = 'W';

              b->board[x][y + 1]->taggedWall = true;
              b->board[x][y + 1]->icon = 'W';

              b->board[x][y + 2]->taggedWall = true;
              b->board[x][y + 2]->icon = 'W';
            }
        }
    }
}

bool
sendBackupUnits (board *b, int amount, hero *h)
{
  if (b->backupUnits < amount)
    return false;
  for (int i = 0; i < amount; ++i)
    {
      bool response = false;
      while (!response)
        {
          unitPrototype *up = &h->protoList[rand () % 3];
          unit *u = newUnitFromProto (up, randColor ());
          int random = rand () % b->WIDTH;
          response = sendUnit (b, u, random);
          if (response)
            b->backupUnits--;
        }
    }
  return true;
}

void
tagAttacks3x1 (board *b)
{
  for (int y = 0; y < b->WIDTH; ++y)
    {
      for (int x = 0; x < b->HIGHT - 2; ++x)
        {
          if (!b->board[x][y])
            continue;
          if (cmpUnits (b->board[x][y], b->board[x + 1][y])
              && cmpUnits (b->board[x][y], b->board[x + 2][y])
              && !b->board[x][y]->taggedAttack
              && !b->board[x + 1][y]->taggedAttack
              && !b->board[x + 2][y]->taggedAttack)
            {
              b->board[x][y]->taggedAttack = true;
              b->board[x][y]->icon = 'X';

              b->board[x + 1][y]->taggedAttack = true;
              b->board[x + 1][y]->icon = 'X';

              b->board[x + 2][y]->taggedAttack = true;
              b->board[x + 2][y]->icon = 'X';
            }
        }
    }
}

bool
checkWall (board *b)
{
  for (int x = 0; x < b->HIGHT; ++x)
    {
      for (int y = 0; y < b->WIDTH - 2; ++y)
        {
          if (!b->board[x][y])
            continue;
          if (cmpUnits (b->board[x][y], b->board[x][y + 1])
              && cmpUnits (b->board[x][y], b->board[x][y + 2]))
            return true;
        }
    }
  return false;
}

bool
checkAttack3x1 (board *b)
{
  for (int y = 0; y < b->WIDTH; ++y)
    {
      for (int x = 0; x < b->HIGHT - 2; ++x)
        {
          if (!b->board[x][y])
            continue;
          if (cmpUnits (b->board[x][y], b->board[x + 1][y])
              && cmpUnits (b->board[x][y], b->board[x + 2][y])
              && !b->board[x][y]->taggedAttack
              && !b->board[x + 1][y]->taggedAttack
              && !b->board[x + 2][y]->taggedAttack)
            return true;
        }
    }
  return false;
}

bool
checkTagsNeeded (board *b)
{
  return false;
  return checkWall (b) || checkAttack3x1 (b);
}

void
makeWalls (board *b, hero *h)
{
  for (int y = 0; y < b->WIDTH; ++y)
    {
      for (int x = 0; x < b->HIGHT - 2; ++x)
        {
          if (b->board[x][y]->taggedWall)
            {
            }
        }
    }
}