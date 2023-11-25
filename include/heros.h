#ifndef _HEROS_H_
#define _HEROS_H_

#include "sprites.h"
#include "units.h"

#define UNIT_COUNT 3

typedef enum heroType
{
  paladin = 0,
  mechanic = 1,
  wizard = 2
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

  heroAnimationDatabase *animationDb;

  // 0 is red, 1 is green, 2 is blue
  unitPrototype activeUnits[3];
} hero;

hero *newHero (heroType hc, char *n, int maxUnits);

void freeHero (hero *h);

formationPrototype3x1 *getFormationPrototype (hero *hero, unitType type);

unit getRandomUnit (hero *h);

void printHero (hero *h);

#endif
