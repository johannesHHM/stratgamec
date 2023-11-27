#include "match.h"
#include "board.h"
#include "heros.h"
#include "types.h"

#include "raylib.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

match *
newMatch (hero *hero1, hero *hero2)
{
  match *m = malloc (sizeof (match));
  m->board1 = newBoard (hero1->maxUnits);
  m->board2 = newBoard (hero2->maxUnits);

  m->hero1 = hero1;
  m->hero2 = hero2;
  
  // TODO cursor point start x?
  m->cursorPosition = (point){ 20, 3 };
  m->hasUnitSelected = false;
  m->selectedPosition = (point){ -1, -1 };

  m->debug = false;

  for (int d = 0; d < 4; d++)
    {
      m->movingDirection[d] = false;
      m->cooldownCounter[d] = HOLD_COOLDOWN;
      m->pauseCounter[d] = HOLD_PAUSE;
    }

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
  bool skipRest = false;
  for (int x = 0; x < 6; ++x)
    {
      for (int y = 0; y < 8; ++y)
        {
          unit *u;
          u = &match->board1->board[x][y];
          if (!u->occupied)
            continue;
          if (u->animData.state == poof)
            {
              tickUnitAnimationData (u);
              skipRest = true;
            }
        }
    }
  if (skipRest)
    goto skip;

  for (int x = 0; x < 6; ++x)
    {
      for (int y = 0; y < 8; ++y)
        {
          unit *u;
          u = &match->board1->board[x][y];
          if (!u->occupied)
            continue;

          tickUnitAnimationData (u);
          // skipRest = true;
        }
    }

skip:

  // match->board1->backupUnits = 1;
  // usleep (50 * 1000);

  // handleInput (match);

  updateHoldState (match);

  if (checkLockedAnimations (match->board1))
    {
      handleInputLocked (match);
      return;
    }
  else
    {

      bool updated = false;
      // TODO remove this thing
      bool test = false;
      /* TODO this has to loop */
      // TODO animate when remove units (how to play the naimation when the
      // unit is gone hmmm)

      // TODO animations here! (walking animation)
      updated = sinkUnits (match->board1);
      // fprintf (stderr, "1st %d\n", updated);
      if (updated)
        goto skip_rest;

      tagWalls (match->board1);
      tagAttacks3x1 (match->board1);

      // TODO animations here! (make wall animation)
      updated = makeWalls (match->board1, match->hero1);
      // fprintf (stderr, "2nd %d\n", updated);
      if (updated && test)
        goto skip_rest;

      // TODO animations here! (make wall walking lul) (make moved units
      // walk)
      updated = sinkWalls (match->board1);
      // fprintf (stderr, "3rd %d\n", updated);
      if (updated && test)
        goto skip_rest;

      // TODO animations here! (make attack animation)
      updated = makeAttacks3x1 (match->board1, match->hero1);
      // fprintf (stderr, "4th %d\n", updated);
      if (updated && test)
        goto skip_rest;

      // TODO animations here! (make walk animation) (make moved units
      // walk)
      updated = sinkAttacks3x1 (match->board1);
      // fprintf (stderr, "5th %d\n", updated);
      if (updated && test)
        goto skip_rest;

      // printBoard (match->board1);
    }
  handleInput (match);

skip_rest:
}

void
handleInputLocked (match *m)
{
  if (canMoveCursor (m, LEFT))
    {
      moveCursor (m, LEFT);
    }
  else if (canMoveCursor (m, RIGHT))
    {
      moveCursor (m, RIGHT);
    }
  else if (canMoveCursor (m, UP))
    {
      moveCursor (m, UP);
    }
  else if (canMoveCursor (m, DOWN))
    {
      moveCursor (m, DOWN);
    }

  if (IsKeyPressed (KEY_D))
    m->debug = !m->debug;
}

void
handleInput (match *m)
{
  if (!m->hasUnitSelected)
    {
      if (canMoveCursor (m, LEFT))
        {
          moveCursor (m, LEFT);
        }
      else if (canMoveCursor (m, RIGHT))
        {
          moveCursor (m, RIGHT);
        }
      else if (canMoveCursor (m, UP))
        {
          moveCursor (m, UP);
        }
      else if (canMoveCursor (m, DOWN))
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
      else if (IsKeyPressed (KEY_LEFT_CONTROL)
               || IsKeyPressed (KEY_RIGHT_CONTROL))
        {
          deleteUnit (m);
        }
      else if (IsKeyPressed (KEY_TAB))
        {
          sendBackupUnits (m->board1, m->hero1);
          setCursorTop (m);
        }
    }
  else
    {
      if ((IsKeyPressed (KEY_LEFT) || m->pauseCounter[LEFT] == 0)
          && !m->movingDirection[RIGHT] && m->selectedPosition.y > 0)
        {
          m->selectedPosition.y--;
          m->selectedPosition.x
              = getTopUnit (m->board1, m->selectedPosition.y).x + 1;
        }
      else if ((IsKeyPressed (KEY_RIGHT) || m->pauseCounter[RIGHT] == 0)
               && !m->movingDirection[LEFT]
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
              int x = getTopUnit (m->board1, m->selectedPosition.y).x;
              unit *u = &m->board1->board[x][m->selectedPosition.y];
              setUnitAnimationState (u, walking, ((WIDTH_C - x) * 32), 0,
                                     true);
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
updateHoldState (match *m)
{
  for (int d = 0; d < 4; d++)
    {
      if (IsKeyDown (KEY_DIRECTION_START + d) && m->movingDirection[d])
        {
          m->pauseCounter[d]--;
          if (m->pauseCounter[d] < -1)
            {
              m->pauseCounter[d] = HOLD_PAUSE;
            }
        }
      if (IsKeyDown (KEY_DIRECTION_START + d) && !m->movingDirection[d])
        {
          if (m->cooldownCounter[d] <= 0)
            {
              m->pauseCounter[d] = HOLD_PAUSE;
              m->movingDirection[d] = true;
            }
          else
            {
              m->cooldownCounter[d]--;
            }
        }
      if (!IsKeyDown (KEY_DIRECTION_START + d))
        {
          m->pauseCounter[d] = HOLD_PAUSE;
          m->cooldownCounter[d] = HOLD_COOLDOWN;
          m->movingDirection[d] = false;
        }
    }
}

bool
canMoveCursor (match *m, direction d)
{
  switch (d)
    {
    case (UP):
      return (IsKeyPressed (KEY_UP) || m->pauseCounter[UP] == 0)
             && !IsKeyDown (KEY_DOWN) && m->cursorPosition.x > 0;
    case (DOWN):
      return (IsKeyPressed (KEY_DOWN) || m->pauseCounter[DOWN] == 0)
             && !IsKeyDown (KEY_UP)
             && m->cursorPosition.x < m->board1->HIGHT - 1
             && m->cursorPosition.x
                    < getTopUnit (m->board1, m->cursorPosition.y).x;
    case (LEFT):
      return (IsKeyPressed (KEY_LEFT) || m->pauseCounter[LEFT] == 0)
             && !IsKeyDown (KEY_RIGHT) && m->cursorPosition.y > 0;
    case RIGHT:
      return (IsKeyPressed (KEY_RIGHT) || m->pauseCounter[RIGHT] == 0)
             && !IsKeyDown (KEY_LEFT)
             && m->cursorPosition.y < m->board1->WIDTH - 1;
    }
  return false;
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
      setCursorTop (m);
      break;
    case RIGHT:
      m->cursorPosition.y++;
      setCursorTop (m);
      break;
    }
}

void
setCursorTop (match *m)
{
  m->cursorPosition.x = getTopUnit (m->board1, m->cursorPosition.y).x;
  if (m->cursorPosition.x == -1)
    m->cursorPosition.x = 0;
}

void
deleteUnit (match *m)
{
  if (!m->board1->board[m->cursorPosition.x][m->cursorPosition.y].occupied)
    return;
  if (canBeRemoved (
          &m->board1->board[m->cursorPosition.x][m->cursorPosition.y]))
    {
      point top;

      top = getTopUnit (m->board1, m->cursorPosition.y);
      setUnitAnimationState (
          &m->board1->board[m->cursorPosition.x][m->cursorPosition.y], poof,
          GetRandomValue (-2, 2), GetRandomValue (-2, 2), true);

      // res = removeUnit (m->board1, m->cursorPosition.x,
      // m->cursorPosition.y);

      m->board1->backupUnits
          += m->board1->board[m->cursorPosition.x][m->cursorPosition.y]
                 .backupValue;

      // if (res)
      //   m->board1->backupUnits++;
      // else
      //   return;

      if (m->cursorPosition.x == top.x && m->cursorPosition.x > 0)
        m->cursorPosition.x--;
    }
}
