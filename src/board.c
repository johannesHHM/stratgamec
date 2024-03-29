#include "board.h"
#include "heros.h"
#include "types.h"
#include "units.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
                  else if (b->board[x][y].hasFormation)
                    printf (" %ch", b->board[x][y].tagIcon);
                  else
                    printf (" %cn", b->board[x][y].tagIcon);
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
    return false;

  int x = b->HIGHT - 1;

  while (x >= 0 && !b->board[x][y].occupied)
    x--;
  // TODO im settign this shit twice rn, kinda meh
  setUnitAnimationState (&u, walking, (WIDTH_C - x + 1) * UNIT_SIZE, 0, true);
  b->board[x + 1][y] = u;

  return true;
}

bool
sendUnitBackup (board *b, unit u, int y)
{
  if (b->board[b->HIGHT - 1][y].occupied)
    return false;

  int x = b->HIGHT - 1;

  while (x >= 0 && !b->board[x][y].occupied)
    x--;

  setUnitAnimationState (&u, walking, (WIDTH_C + 1) * UNIT_SIZE, 0, true);
  b->board[x + 1][y] = u;
  if (checkTagsNeeded (b))
    {
      b->board[x + 1][y].occupied = false;
      return false;
    }

  return true;
}

// TODO if all units that can be sent creates
// a wall or attack, will loop forever
bool
sendBackupUnits (board *b, hero *h)
{
  if (checkFullBoard (b))
    return false;
  while (b->backupUnits > 0)
    {
      bool response = false;
      while (!response)
        {
          int y = rand () % b->WIDTH;
          response = sendUnitBackup (b, getRandomUnit (h), y);
          if (response)
            b->backupUnits--;
        }
    }
  return true;
}

int
getTopFree (board *b, int y)
{
  if (b->board[b->HIGHT - 1][y].occupied)
    return -1;

  int x = 0;
  while (x < b->HIGHT && b->board[x][y].occupied)
    x++;

  return x - 1;
}

point
getTopUnit (board *b, int y)
{
  if (!b->board[0][y].occupied)
    return (point){ -1, -1 };

  int x = 0;
  while (x < b->HIGHT && b->board[x][y].occupied)
    x++;

  return (point){ x - 1, y };
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
          if (b->board[x][y].hasFormation || b->board[x][y + 1].hasFormation
              || b->board[x][y + 2].hasFormation)
            continue;
          if ((int)b->board[x][y].type < 10
              || (int)b->board[x][y + 1].type < 10
              || (int)b->board[x][y + 2].type < 10)
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
          if ((int)b->board[x][y].type < 10
              || (int)b->board[x + 1][y].type < 10
              || (int)b->board[x + 2][y].type < 10)
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
          if (b->board[x][y].hasFormation || b->board[x][y + 1].hasFormation
              || b->board[x][y + 2].hasFormation)
            continue;
          if ((int)b->board[x][y].type < 10
              || (int)b->board[x][y + 1].type < 10
              || (int)b->board[x][y + 2].type < 10)
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
          if ((int)b->board[x][y].type < 10
              || (int)b->board[x + 1][y].type < 10
              || (int)b->board[x + 2][y].type < 10)
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

// TODO on false always rn
bool
checkTagsNeeded (board *b)
{
  return checkWall (b) || checkAttack3x1 (b);
}

bool
checkFullBoard (board *b)
{
  for (int y = 0; y < b->WIDTH; y++)
    {
      if (!b->board[b->HIGHT - 1][y].occupied)
        return false;
    }
  return true;
}

bool
checkLockedAnimations (board *b)
{
  for (int x = 0; x < b->HIGHT; ++x)
    {
      for (int y = 0; y < b->WIDTH; ++y)
        {
          if (!b->board[x][y].occupied)
            continue;
          if (b->board[x][y].animData.lockedAnimating)
            return true;
        }
    }
  return false;
}

bool
moveUnit (board *b, int x0, int y0, int x1, int y1)
{
  if (x0 == x1 && y0 == y1)
    return true;
  if (!b->board[x0][y0].occupied)
    return false;
  if (b->board[x1][y1].occupied)
    return false;
  b->board[x1][y1] = b->board[x0][y0];
  b->board[x0][y0].occupied = false;
  return true;
}

bool
moveUnitToColumn (board *b, point p0, int y1)
{
  if (!b->board[p0.x][p0.y].occupied)
    return false;

  if (b->board[b->HIGHT - 1][y1].occupied)
    return false;

  int x = b->HIGHT - 1;

  while (x >= 0 && !b->board[x][y1].occupied)
    x--;

  b->board[x + 1][y1] = b->board[p0.x][p0.y];
  b->board[p0.x][p0.y].occupied = false;
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

bool
makeWalls (board *b, hero *h)
{
  bool updates = false;
  for (int y = 0; y < b->WIDTH; ++y)
    {
      for (int x = 0; x < b->HIGHT; ++x)
        {
          if (!b->board[x][y].occupied)
            continue;

          if (b->board[x][y].taggedWall)
            {
              updates = true;
              if (!b->board[x][y].taggedAttack)
                {
                  removeUnit (b, x, y);
                  unit wall;
                  wall
                      = initUnitFromProto (&h->protoWall, red, h->animationDb);
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
                      // TODO wall color?
                      wall = initUnitFromProto (&h->protoWall, 0,
                                                h->animationDb);
                      wall.backupValue = 0;
                      b->board[wallX][y] = wall;
                    }
                }
            }
        }
    }
  return updates;
}

bool
sinkWalls (board *b)
{
  bool updates = false;

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
                  updates = true;
                  updateWalkingUnit (b, x + 1, y, 1);
                  updateWalkingUnit (b, x, y, -1);

                  swapUnits (b, x, y, x + 1, y);
                  updated = true;
                }
            }
        }
    }
  return updates;
}

bool
makeAttacks3x1 (board *b, hero *h)
{
  bool updates = false;

  for (int y = 0; y < b->WIDTH; y++)
    {
      for (int x = 0; x < b->HIGHT - 2; x++)
        {
          if (!b->board[x][y].occupied || !b->board[x][y].firstFormation)
            continue;
          if (b->board[x][y].hasFormation)
            continue;

          updates = true;

          formationPrototype3x1 *fp;
          formation3x1 *f;

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
  return updates;
}

bool
sinkAttacks3x1 (board *b)
{
  bool updates = false;

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
                  updates = true;
                  updateWalkingUnit (b, x + 1, y, 1);
                  updateWalkingUnit (b, x, y, -1);

                  swapUnits (b, x, y, x + 1, y);
                  updated = true;
                }
            }
        }
    }
  return updates;
}

bool
sinkUnits (board *b)
{
  bool updates = false;
  bool updated = true;
  while (updated)
    {
      updated = false;
      for (int y = 0; y < b->WIDTH; y++)
        {
          for (int x = 0; x < b->HIGHT - 1; x++)
            {
              if (!b->board[x][y].occupied && b->board[x + 1][y].occupied)
                {
                  updates = true;
                  updateWalkingUnit (b, x + 1, y, 1);
                  moveUnit (b, x + 1, y, x, y);
                  updated = true;
                }
            }
        }
    }
  return updates;
}

/* Direction where 1 is up and -1 is down */
void
updateWalkingUnit (board *b, int x, int y, int direction)
{
  if (b->board[x][y].animData.state == walking)
    {
      b->board[x][y].animData.offX += UNIT_SIZE * direction;
    }
  else
    {
      setUnitAnimationState (&b->board[x][y], walking, UNIT_SIZE * direction,
                             0, true);
    }
}
