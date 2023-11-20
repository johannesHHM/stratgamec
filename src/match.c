#include "match.h"
#include "board.h"
#include "heros.h"
#include "types.h"

#include "raylib.h"

#include <unistd.h>

match *
newMatch (hero *hero1, hero *hero2)
{
  match *m = malloc (sizeof (match));
  m->board1 = newBoard (hero1->maxUnits);
  m->board2 = newBoard (hero2->maxUnits);

  m->hero1 = hero1;
  m->hero2 = hero2;

  m->cursorPosition = (point){ -1, 3 };
  m->hasUnitSelected = false;
  m->selectedPosition = (point){ -1, -1 };

  m->debug = false;

  return m;
}

void
freeMatch (match *m)
{
  freeBoard (m->board1);
  freeBoard (m->board2);
  free (m);
  m = NULL;
}

void
runMatch (match *match)
{
  // match->board1->backupUnits = 1;
  // usleep (500 * 1000);

  handleInput (match);

  /* TODO this has to loop */

  // printBoard (match->board1);

  sinkUnits (match->board1);

  tagWalls (match->board1);
  tagAttacks3x1 (match->board1);

  // printBoard (match->board1);

  makeWalls (match->board1, match->hero1);

  // printBoard (match->board1);

  sinkWalls (match->board1);

  // printBoard (match->board1);

  makeAttacks3x1 (match->board1, match->hero1);

  // printBoard (match->board1);

  sinkAttacks3x1 (match->board1);

  printBoard (match->board1);
}

void
handleInput (match *m)
{
  if (!m->hasUnitSelected)
    {
      if (IsKeyPressed (KEY_LEFT) && m->cursorPosition.y > 0)
        {
          moveCursor (m, LEFT);
        }
      if (IsKeyPressed (KEY_RIGHT)
          && m->cursorPosition.y < m->board1->WIDTH - 1)
        {
          moveCursor (m, RIGHT);
        }
      if (IsKeyPressed (KEY_UP) && m->cursorPosition.x > 0)
        {
          moveCursor (m, UP);
        }
      if (IsKeyPressed (KEY_DOWN) && m->cursorPosition.x < m->board1->HIGHT - 1
          && m->cursorPosition.x
                 < getTopUnit (m->board1, m->cursorPosition.y).x)
        {
          moveCursor (m, DOWN);
        }
      if (IsKeyPressed (KEY_SPACE))
        {
          point p = getTopUnit (m->board1, m->cursorPosition.y);
          if (p.x != -1 && canBeMoved (&m->board1->board[p.x][p.y]))
            {
              m->cursorPosition = p;
              m->selectedUnit = m->board1->board[p.x][p.y];
              removeUnit (m->board1, p.x, p.y);
              m->hasUnitSelected = true;
              m->selectedPosition = p;
            }
        }
      if (IsKeyPressed (KEY_LEFT_CONTROL) || IsKeyPressed (KEY_RIGHT_CONTROL))
        {
          deleteUnit (m);
        }
      if (IsKeyPressed (KEY_TAB))
        {
          sendBackupUnits (m->board1, m->hero1);
        }
    }
  else
    {
      if (IsKeyPressed (KEY_LEFT) && m->selectedPosition.y > 0)
        {
          m->selectedPosition.y--;
          m->selectedPosition.x
              = getTopUnit (m->board1, m->selectedPosition.y).x + 1;
        }
      if (IsKeyPressed (KEY_RIGHT)
          && m->selectedPosition.y < m->board1->WIDTH - 1)
        {
          m->selectedPosition.y++;
          m->selectedPosition.x
              = getTopUnit (m->board1, m->selectedPosition.y).x + 1;
        }
      if (IsKeyPressed (KEY_SPACE))
        {
          bool succ;
          succ = sendUnit (m->board1, m->selectedUnit, m->selectedPosition.y);
          if (succ)
            {
              m->hasUnitSelected = false;
              m->cursorPosition = m->selectedPosition;
              m->selectedPosition = (point){ -1, -1 };
            }
        }
    }

  if (IsKeyPressed (KEY_D))
    m->debug = !m->debug;
}

void
moveCursor (match *m, direction d)
{
  switch (d)
    {
    case (UP):
      m->cursorPosition.x--;
      break;
    case (DOWN):
      m->cursorPosition.x++;
      break;
    case (LEFT):
      m->cursorPosition.y--;
      m->cursorPosition.x = getTopUnit (m->board1, m->cursorPosition.y).x;
      if (m->cursorPosition.x == -1)
        m->cursorPosition.x = 0;
      break;
    case RIGHT:
      m->cursorPosition.y++;
      m->cursorPosition.x = getTopUnit (m->board1, m->cursorPosition.y).x;
      if (m->cursorPosition.x == -1)
        m->cursorPosition.x = 0;
      break;
    }
}

void
deleteUnit (match *m)
{
  if (canBeRemoved (
          &m->board1->board[m->cursorPosition.x][m->cursorPosition.y]))
    {
      point top;
      bool res;

      top = getTopUnit (m->board1, m->cursorPosition.y);
      res = removeUnit (m->board1, m->cursorPosition.x, m->cursorPosition.y);

      if (res)
        m->board1->backupUnits++;
      else
        return;

      if (m->cursorPosition.x == top.x && m->cursorPosition.x > 0)
        m->cursorPosition.x--;
    }
}