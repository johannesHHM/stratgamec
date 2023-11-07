#include "board.h"
#include "game.h"
#include "heros.h"
#include "sprites.h"
#include "units.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void
test ()
{
  board *b = newBoard (25);
  hero *h = newHero (paladin, "Uther", 25);

  unit redArcher0 = initUnitFromProto (&h->unitProtoList[0], 0);
  unit redArcher1 = initUnitFromProto (&h->unitProtoList[0], 0);
  unit redArcher2 = initUnitFromProto (&h->unitProtoList[0], 0);
  unit redArcher3 = initUnitFromProto (&h->unitProtoList[0], 0);
  unit redArcher4 = initUnitFromProto (&h->unitProtoList[0], 0);

  unit greenArcher0 = initUnitFromProto (&h->unitProtoList[0], 1);
  unit greenArcher1 = initUnitFromProto (&h->unitProtoList[0], 1);
  unit greenArcher2 = initUnitFromProto (&h->unitProtoList[0], 1);
  unit greenArcher3 = initUnitFromProto (&h->unitProtoList[0], 1);

  // unit blueArcher0 = initUnitFromProto (&h->unitProtoList[0], 2);
  // unit blueArcher1 = initUnitFromProto (&h->unitProtoList[0], 2);
  // unit blueArcher2 = initUnitFromProto (&h->unitProtoList[0], 2);

  sendUnit (b, redArcher0, 0);
  sendUnit (b, redArcher1, 0);
  sendUnit (b, redArcher2, 0);

  // sendUnit (b, blueArcher0, 0);
  // sendUnit (b, blueArcher1, 0);
  // sendUnit (b, blueArcher2, 0);

  sendUnit (b, greenArcher0, 1);
  sendUnit (b, greenArcher1, 2);
  sendUnit (b, greenArcher2, 1);
  sendUnit (b, greenArcher3, 2);

  sendUnit (b, redArcher3, 1);
  sendUnit (b, redArcher4, 2);

  printBoard (b);

  tagWalls (b);
  tagAttacks3x1 (b);

  printBoard (b);

  makeWalls (b, h);

  printBoard (b);

  sinkWalls (b);

  printBoard (b);
  exit (0);
}

void
testAnim ()
{
  SetTraceLogLevel (LOG_ERROR);

  hero *h = newHero (paladin, "Uther", 25);

  heroAnimationDatabase *db = readHeroAnimationDatabase ((int)h->type);

  printHeroAnimationDatabase (db);

  freeHeroAnimationDatabase (db);

  freeHero (h);

  exit (0);
}

int
main ()
{
  testAnim ();
  // runGame ();
  exit (0);
}
