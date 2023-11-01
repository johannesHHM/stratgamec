#include "board.h"
#include "heros.h"
#include "units.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// TODO change backup units to hero ?
board *
newBoard (int maxUnits)
{
  board *b = (board *)malloc (sizeof (board));
  b->HIGHT = WIDTH_C;
  b->WIDTH = HIGHT_C;
  for (int x = 0; x < b->HIGHT; ++x)
    {
      for (int y = 0; y < b->WIDTH; ++y)
        {
          b->board[x][y].occupied = false;
        }
    }
  b->backupUnits = maxUnits;
  return b;
}

void
freeBoard (board *b)
{
  free (b);
  b = NULL;
}

void
printBoard (board *b)
{
  for (int i = 0; i < b->HIGHT * 3 / 2; i++)
    {
      printf (" ");
    }
  printf ("Board\n");
  for (int x = 0; x < b->HIGHT; ++x)
    {
      printf ("\n");
      for (int y = 0; y < b->WIDTH; ++y)
        {
          if (b->board[x][y].occupied)
            {
              setPrintColor (b->board[x][y].color);
              if (b->board[x][y].tagIcon == 'n')
                {
                  printf (" %c ", b->board[x][y].icon);
                }
              else
                {
                  if (b->board[x][y].firstFormation)
                    printf (" %cf", b->board[x][y].tagIcon);
                  else
                    printf (" %c ", b->board[x][y].tagIcon);
                }
            }
          else
            {
              setPrintColor (def);
              printf (" _ ");
            }
        }
    }
  setPrintColor (def);
  printf ("\n\n");
}

bool
placeUnit (board *b, unit u, int x, int y)
{
  if (b->board[x][y].occupied)
    {
      return false;
    }
  b->board[x][y] = u;
  return true;
}

bool
sendUnit (board *b, unit u, int y)
{
  if (b->board[b->HIGHT - 1][y].occupied)
    {
      return false;
    }
  int x = b->HIGHT - 1;
  while (x >= 0 && !b->board[x][y].occupied)
    {
      x--;
    }
  b->board[x + 1][y] = u;
  if (checkTagsNeeded (b))
    {
      b->board[x + 1][y].occupied = false;
      return false;
    }
  return true;
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
          unit u = initUnitFromProto (&h->unitProtoList[rand () % 3],
                                      randColor ());
          int random = rand () % b->WIDTH;
          response = sendUnit (b, u, random);
          if (response)
            b->backupUnits--;
        }
    }
  return true;
}

bool
removeUnit (board *b, int x, int y)
{
  if (!b->board[x][y].occupied)
    return false;
  b->board[x][y].occupied = false;
  return true;
}

void
tagWalls (board *b)
{
  for (int x = 0; x < b->HIGHT; ++x)
    {
      for (int y = 0; y < b->WIDTH - 2; ++y)
        {
          if (!b->board[x][y].occupied || !b->board[x][y + 1].occupied
              || !b->board[x][y + 2].occupied)
            continue;
          if (cmpUnits (&b->board[x][y], &b->board[x][y + 1])
              && cmpUnits (&b->board[x][y], &b->board[x][y + 2]))
            {
              b->board[x][y].taggedWall = true;
              b->board[x][y].tagIcon = 'W';

              b->board[x][y + 1].taggedWall = true;
              b->board[x][y + 1].tagIcon = 'W';

              b->board[x][y + 2].taggedWall = true;
              b->board[x][y + 2].tagIcon = 'W';
            }
        }
    }
}

void
tagAttacks3x1 (board *b)
{
  for (int y = 0; y < b->WIDTH; ++y)
    {
      for (int x = 0; x < b->HIGHT - 2; ++x)
        {
          if (!b->board[x][y].occupied || !b->board[x + 1][y].occupied
              || !b->board[x + 2][y].occupied)
            continue;
          if (cmpUnits (&b->board[x][y], &b->board[x + 1][y])
              && cmpUnits (&b->board[x][y], &b->board[x + 2][y])
              && !b->board[x][y].taggedAttack
              && !b->board[x + 1][y].taggedAttack
              && !b->board[x + 2][y].taggedAttack)
            {
              b->board[x][y].taggedAttack = true;
              if (b->board[x][y].tagIcon == 'n')
                b->board[x][y].tagIcon = 'X';
              else if (b->board[x][y].tagIcon == 'W')
                b->board[x][y].tagIcon = 'D';

              b->board[x][y].firstFormation = true;

              b->board[x + 1][y].taggedAttack = true;
              if (b->board[x + 1][y].tagIcon == 'n')
                b->board[x + 1][y].tagIcon = 'X';
              else if (b->board[x + 1][y].tagIcon == 'W')
                b->board[x + 1][y].tagIcon = 'D';

              b->board[x + 2][y].taggedAttack = true;
              if (b->board[x + 2][y].tagIcon == 'n')
                b->board[x + 2][y].tagIcon = 'X';
              else if (b->board[x + 2][y].tagIcon == 'W')
                b->board[x + 2][y].tagIcon = 'D';
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
          if (!b->board[x][y].occupied || !b->board[x][y + 1].occupied
              || !b->board[x][y + 2].occupied)
            continue;
          if (cmpUnits (&b->board[x][y], &b->board[x][y + 1])
              && cmpUnits (&b->board[x][y], &b->board[x][y + 2]))
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
          if (!b->board[x][y].occupied || !b->board[x + 1][y].occupied
              || !b->board[x + 2][y].occupied)
            continue;
          if (cmpUnits (&b->board[x][y], &b->board[x + 1][y])
              && cmpUnits (&b->board[x][y], &b->board[x + 2][y])
              && !b->board[x][y].taggedAttack
              && !b->board[x + 1][y].taggedAttack
              && !b->board[x + 2][y].taggedAttack)
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

bool
moveUnit (board *b, int x0, int y0, int x1, int y1)
{
  if (!b->board[x0][y0].occupied)
    return false;
  if (b->board[x1][y1].occupied)
    return false;
  b->board[x1][y1] = b->board[x0][y0];
  b->board[x0][y0].occupied = false;
  return true;
}

bool
swapUnits (board *b, int x0, int y0, int x1, int y1)
{
  if (!b->board[x0][y0].occupied)
    return false;
  if (!b->board[x1][y1].occupied)
    return false;
  unit tmpUnit = b->board[x1][y1];
  b->board[x1][y1] = b->board[x0][y0];
  b->board[x0][y0] = tmpUnit;
  return true;
}

int
handleDoubleTag (board *b, int x, int y)
{
  b->board[x][y].taggedWall = false;
  b->board[x][y].tagIcon = 'X';

  if (b->board[b->HIGHT - 1][y].occupied)
    return -1;

  int xFormStart;
  for (int i = x; i >= 0; i--)
    {
      if (b->board[i][y].firstFormation)
        {
          xFormStart = i;
          break;
        }
    }

  for (int i = b->HIGHT - 1; i > xFormStart; i--)
    moveUnit (b, i - 1, y, i, y);

  return xFormStart;
}

void
makeWalls (board *b, hero *h)
{
  for (int y = 0; y < b->WIDTH; ++y)
    {
      for (int x = 0; x < b->HIGHT; ++x)
        {
          if (!b->board[x][y].occupied)
            continue;

          if (b->board[x][y].taggedWall)
            {
              if (!b->board[x][y].taggedAttack)
                {
                  removeUnit (b, x, y);
                  unit wall;
                  wall = initUnitFromProto (&h->protoWall, def);
                  b->board[x][y] = wall;
                }
              else
                {
                  int wallX;
                  wallX = handleDoubleTag (b, x, y);
                  if (wallX > -1)
                    {
                      removeUnit (b, wallX, y);
                      unit wall;
                      wall = initUnitFromProto (&h->protoWall, def);
                      b->board[wallX][y] = wall;
                    }
                }
            }
        }
    }
}

void
sinkWalls (board *b)
{
  bool updated = true;
  while (updated)
    {
      updated = false;
      for (int y = 0; y < b->WIDTH; ++y)
        {
          for (int x = 0; x < b->HIGHT - 1; ++x)
            {
              if (!b->board[x][y].occupied || !b->board[x + 1][y].occupied)
                continue;

              if ((int)b->board[x][y].type > 9
                  && (int)b->board[x + 1][y].type < 10)
                {
                  swapUnits (b, x, y, x + 1, y);
                  updated = true;
                }
            }
        }
    }
}

void
makeAttacks3x1 (board *b, hero *h)
{
  for (int y = 0; y < b->WIDTH; y++)
    {
      for (int x = 0; x < b->HIGHT - 2; x++)
        {
          if (!b->board[x][y].occupied || !b->board[x][y].firstFormation)
            continue;

          formation3x1 *f;
          formationPrototype3x1 *fp;

          fp = getFormationPrototype (h, b->board[x][y].type);
          f = newFormationFromProto3x1 (fp, b->board[x][y].color);

          b->board[x][y].formation3x1 = f;
          b->board[x][y].firstFormation = true;
          b->board[x][y].hasFormation = true;

          b->board[x + 1][y].hasFormation = true;
          b->board[x + 2][y].hasFormation = true;

          b->board[x][y].tagIcon = toupper (b->board[x][y].icon);
          b->board[x + 1][y].tagIcon = toupper (b->board[x + 1][y].icon);
          b->board[x + 2][y].tagIcon = toupper (b->board[x + 2][y].icon);
        }
    }
}

void
sinkAttacks3x1 (board *b)
{
  bool updated = true;
  while (updated)
    {
      updated = false;
      for (int y = 0; y < b->WIDTH; ++y)
        {
          for (int x = 0; x < b->HIGHT - 1; ++x)
            {
              if (!b->board[x][y].occupied || !b->board[x + 1][y].occupied)
                continue;

              if ((int)b->board[x][y].type < 10)
                continue;

              if (!b->board[x][y].hasFormation
                  && b->board[x + 1][y].hasFormation)
                {
                  swapUnits (b, x, y, x + 1, y);
                  updated = true;
                }
            }
        }
    }
}