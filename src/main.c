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
  //   InitWindow (100, 100, "temp_window_title");

  //   board *b = newBoard (25);
  //   hero *h = newHero (paladin, "Uther", 25);

  //   unit redArcher0
  //       = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  //   unit redArcher1
  //       = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  //   unit redArcher2
  //       = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  //   unit redArcher3
  //       = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  //   unit redArcher4
  //       = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);

  //   unit redArcher5
  //       = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  //   unit redArcher6
  //       = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);
  //   unit redArcher7
  //       = initUnitFromProto (&h->unitProtoList[0], 0, h->animationDb);

  //   unit greenArcher0
  //       = initUnitFromProto (&h->unitProtoList[0], 1, h->animationDb);
  //   unit greenArcher1
  //       = initUnitFromProto (&h->unitProtoList[0], 1, h->animationDb);
  //   unit greenArcher2
  //       = initUnitFromProto (&h->unitProtoList[0], 1, h->animationDb);
  //   unit greenArcher3
  //       = initUnitFromProto (&h->unitProtoList[0], 1, h->animationDb);

  //   unit blueArcher0
  //       = initUnitFromProto (&h->unitProtoList[0], 2, h->animationDb);
  //   unit blueArcher1
  //       = initUnitFromProto (&h->unitProtoList[0], 2, h->animationDb);
  //   unit blueArcher2
  //       = initUnitFromProto (&h->unitProtoList[0], 2, h->animationDb);

  //   sendUnit (b, redArcher0, 0);
  //   sendUnit (b, redArcher1, 1);
  //   sendUnit (b, redArcher2, 2);

  //   sinkUnits (b);
  //   tagWalls (b);
  //   tagAttacks3x1 (b);
  //   makeWalls (b, h);
  //   sinkWalls (b);
  //   makeAttacks3x1 (b, h);
  //   sinkAttacks3x1 (b);

  //   sendUnit (b, redArcher0, 0);
  //   sendUnit (b, redArcher1, 1);
  //   sendUnit (b, redArcher2, 2);

  //   sinkUnits (b);
  //   tagWalls (b);
  //   tagAttacks3x1 (b);
  //   makeWalls (b, h);
  //   sinkWalls (b);
  //   makeAttacks3x1 (b, h);
  //   sinkAttacks3x1 (b);

  //   sendUnit (b, greenArcher0, 0);
  //   sendUnit (b, greenArcher1, 1);
  //   sendUnit (b, greenArcher2, 2);

  //   sinkUnits (b);
  //   tagWalls (b);
  //   tagAttacks3x1 (b);
  //   makeWalls (b, h);
  //   sinkWalls (b);
  //   makeAttacks3x1 (b, h);
  //   sinkAttacks3x1 (b);

  //   printBoard (b);

  //   sendUnit (b, blueArcher0, 0);
  //   sendUnit (b, blueArcher1, 1);
  //   sendUnit (b, blueArcher2, 2);

  //   printBoard (b);

  //   sinkUnits (b);
  //   tagWalls (b);
  //   //   tagAttacks3x1 (b);
  //   //   makeWalls (b, h);
  //   //   sinkWalls (b);
  //   //   makeAttacks3x1 (b, h);
  //   //   sinkAttacks3x1 (b);

  //   //   // sendUnit (b, blueArcher0, 0);
  //   //   // sendUnit (b, blueArcher1, 0);
  //   //   // sendUnit (b, blueArcher2, 0);

  //   //   sendUnit (b, redArcher5, 1);
  //   //   sendUnit (b, redArcher6, 2);
  //   //   // sendUnit (b, greenArcher2, 1);
  //   //   // sendUnit (b, greenArcher3, 2);

  //   printBoard (b);
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
