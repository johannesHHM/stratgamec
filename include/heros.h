#ifndef _HEROS_H_
#define _HEROS_H_

#include "units.h"

typedef enum heroType
{
  paladin,
  mechanic,
  wizard
} heroType;

typedef struct Hero
{
  heroType type;
  char name[20];
  unitPrototype protoList[3];
  unitPrototype protoWall;
  int level1Wall, level2Wall, level3Wall;
} hero;

hero *newHero (heroType hc, char *n);

void freeHero (hero *h);

void printHero (hero *h);

#endif
