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
testBoard ()
{
  InitWindow (100, 100, "temp_window_title");

  board *b = newBoard (25);
  hero *h = newHero (paladin, "Uther", 25);

  unit redArcher0
      = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  unit redArcher1
      = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  unit redArcher2
      = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  unit redArcher3
      = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  unit redArcher4
      = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);

  unit redArcher5
      = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  unit redArcher6
      = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  unit redArcher7
      = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);

  unit greenArcher0
      = initUnitFromProto (&h->unitProtoList[0], 1, h->animationDb);
  unit greenArcher1
      = initUnitFromProto (&h->unitProtoList[0], 1, h->animationDb);
  unit greenArcher2
      = initUnitFromProto (&h->unitProtoList[0], 1, h->animationDb);
  unit greenArcher3
      = initUnitFromProto (&h->unitProtoList[0], 1, h->animationDb);

  // unit blueArcher0 = initUnitFromProto (&h->unitProtoList[0], 2);
  // unit blueArcher1 = initUnitFromProto (&h->unitProtoList[0], 2);
  // unit blueArcher2 = initUnitFromProto (&h->unitProtoList[0], 2);

  sendUnit (b, redArcher0, 0);
  sendUnit (b, redArcher1, 0);
  sendUnit (b, redArcher2, 0);

  // sendUnit (b, blueArcher0, 0);
  // sendUnit (b, blueArcher1, 0);
  // sendUnit (b, blueArcher2, 0);

  sendUnit (b, redArcher5, 1);
  sendUnit (b, redArcher6, 2);
  // sendUnit (b, greenArcher2, 1);
  // sendUnit (b, greenArcher3, 2);

  printBoard (b);

  tagWalls (b);
  tagAttacks3x1 (b);

  printBoard (b);

  makeWalls (b, h);
  makeAttacks3x1 (b, h);

  printBoard (b);

  sinkWalls (b);

  printBoard (b);

  sendUnit (b, redArcher3, 1);
  sendUnit (b, redArcher4, 2);

  printBoard (b);

  tagWalls (b);
  tagAttacks3x1 (b);

  printBoard (b);

  makeWalls (b, h);
  makeAttacks3x1 (b, h);

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
  // testAnim ();
  // testBoard ();
  runGame ();
  exit (0);
}
