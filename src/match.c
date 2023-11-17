#include "match.h"
#include "board.h"
#include "heros.h"

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

  sendBackupUnits (match->board1, match->board1->backupUnits, match->hero1);

  // printBoard (match->board1);

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

  // usleep (500 * 1000);
}

void
displayMatch (match *match)
{
}
