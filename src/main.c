#include "board.h"
#include "heros.h"
#include "units.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int
main ()
{
  printf ("\nRunning Main\n\n");
  srand (time (NULL));
  board *b = newBoard ();

  hero *hero = newHero (wizard, "Loprix");

  // unit* u1 = newUnitFromProto(&paladinHero->protoList[0], 0);
  // unit* u2 = newUnitFromProto(&paladinHero->protoList[0], 0);
  // unit* u3 = newUnitFromProto(&paladinHero->protoList[0], 0);

  // sendUnit(b, u1, 0);
  // sendUnit(b, u2, 0);
  // sendUnit(b, u3, 0);

  sendBackupUnits (b, 30, hero);

  tagWalls (b);
  tagAttacks3x1 (b);

  printBoard (b);

  tagWalls (b);
  tagAttacks3x1 (b);

  printBoard (b);

  freeHero (hero);
}
