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
  int seed = time (NULL);
  // seed = 1697274596;
  printf ("\nRunning Main\n\n");
  printf ("seed: %d\n", seed);
  srand (seed);
  board *b = newBoard ();

  hero *hero = newHero (paladin, "Uther");
  printHero (hero);

  unit *redArcher0 = newUnitFromProto (&hero->protoList[0], 0);
  unit *redArcher1 = newUnitFromProto (&hero->protoList[0], 0);
  unit *redArcher2 = newUnitFromProto (&hero->protoList[0], 0);
  unit *redArcher3 = newUnitFromProto (&hero->protoList[0], 0);
  unit *redArcher4 = newUnitFromProto (&hero->protoList[0], 0);

  unit *u6 = newUnitFromProto (&hero->protoList[0], 1);
  unit *u7 = newUnitFromProto (&hero->protoList[0], 1);

  sendUnit (b, redArcher0, 0);
  sendUnit (b, redArcher1, 0);

  // moveUnit (b, 1, 0, 2, 0);
  // printBoard (b);
  // exit (0);

  sendUnit (b, redArcher2, 0);

  sendUnit (b, u6, 1);
  sendUnit (b, u7, 2);

  sendUnit (b, redArcher3, 1);
  sendUnit (b, redArcher4, 2);

  // sendBackupUnits (b, 35, hero);

  printBoard (b);

  tagWalls (b);
  tagAttacks3x1 (b);

  printBoard (b);

  makeWalls (b, hero);

  printBoard (b);

  freeHero (hero);
}
