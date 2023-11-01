#include "match.h"
#include "board.h"
#include "heros.h"

match *
newMatch (hero *hero1, hero *hero2)
{
  match *m = malloc (sizeof (match));
  m->board1 = newBoard (hero1->maxUnits);
  m->board2 = newBoard (hero2->maxUnits);
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
runMatch (hero *hero1, hero *hero2)
{
  match *match;

  match = newMatch (hero1, hero2);

  sendBackupUnits (match->board1, hero1->maxUnits, hero1);

  printBoard (match->board1);

  tagWalls (match->board1);
  tagAttacks3x1 (match->board1);

  printBoard (match->board1);

  makeWalls (match->board1, hero1);

  printBoard (match->board1);

  sinkWalls (match->board1);

  printBoard (match->board1);

  makeAttacks3x1 (match->board1, hero1);

  printBoard (match->board1);

  sinkAttacks3x1 (match->board1);

  printBoard (match->board1);

  freeMatch (match);
}