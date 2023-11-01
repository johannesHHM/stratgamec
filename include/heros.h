#ifndef _HEROS_H_
#define _HEROS_H_

#include "units.h"

#define UNIT_COUNT 3

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
  unitPrototype unitProtoList[UNIT_COUNT];
  formationPrototype3x1 formationProtoList[UNIT_COUNT];
  unitPrototype protoWall;
  int level1Wall, level2Wall, level3Wall;

  int maxUnits;
} hero;

hero *newHero (heroType hc, char *n, int maxUnits);

void freeHero (hero *h);

formationPrototype3x1 *getFormationPrototype (hero *hero, unitType type);

void printHero (hero *h);

#endif
