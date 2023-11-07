#ifndef _MATCH_H_
#define _MATCH_H_

#include "board.h"

#include <stdlib.h>

typedef struct Match
{
  board *board1;
  board *board2;

  hero *hero1;
  hero *hero2;

} match;

match *newMatch (hero *hero1, hero *hero2);

void freeMatch (match *m);

void runMatch (match *match);

void displayMatch (match *match);

#endif