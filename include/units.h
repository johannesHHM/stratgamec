#ifndef _UNITS_H_
#define _UNITS_H_

#include "colors.h"

#include <stdbool.h>

typedef struct Formation3x1 formation3x1;

typedef enum unitType
{
  paladinWall = 1,
  mechanicWall = 2,
  wizardWall = 3,

  spearman = 011,
  swordman = 012,
  archer = 013,
  copter = 111,
  engineer = 112,
  bombling = 113
} unitType;

typedef struct Unit
{
  bool occupied;
  unitType type;
  char name[20];
  char icon;
  // TODO maybe debug only?
  char tagIcon;
  color color;

  int strength;

  bool taggedWall;
  bool taggedAttack;
  bool hasFormation;
  bool firstFormation;

  formation3x1 *formation3x1;
} unit;

typedef struct UnitPrototype
{
  unitType type;
  char name[20];
  char icon;
  int strength;
} unitPrototype;

typedef struct FormationPrototype3x1
{
  unitType type;
  int chargeTimer;
  int power;
} formationPrototype3x1;

typedef struct Formation3x1
{
  unitType type;
  int chargeTimer;
  int power;
} formation3x1;

unit *newUnit (unitType t, char n[], char i, color c);
unit *newUnitFromProto (unitPrototype *up, color c);
unit initUnitFromProto (unitPrototype *up, color c);
void initUnit (unit *u, unitType t, char n[], char i, color c);

void freeUnit (unit *u);
void freeFormation3x1 (formation3x1 *f);

bool cmpUnits (unit *u1, unit *u2);

void readUnits (unitPrototype *prototypeList, int ht);
void readWall (unitPrototype *protoWall, int *lvl1Wall, int *lvl2Wall,
               int *lvl3Wall, int ht);

void readFormations3x1 (formationPrototype3x1 *prototypeList, int ht);

formation3x1 *newFormationFromProto3x1 (formationPrototype3x1 *fp, color c);
void freeFormation3x1 (formation3x1 *f);

void printUnit (unit *u);

#endif
