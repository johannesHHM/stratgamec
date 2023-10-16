#ifndef _UNITS_H_
#define _UNITS_H_

#include "colors.h"

#include <stdbool.h>

typedef struct Formation3x1 formation3x1;

typedef enum unitType
{
  paladinWall = 010,
  mechanicWall = 020,
  wizardWall = 030,

  spearman = 011,
  swordman = 012,
  archer = 013,
  copter = 111,
  engineer = 112,
  bombling = 113
} unitType;

typedef struct Unit
{
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

  formation3x1 *formation;
} unit;

typedef struct UnitPrototype
{
  unitType type;
  char name[20];
  char icon;
  int strength;
} unitPrototype;

typedef struct Formation3x1
{
  unitType type;
  unit *units[3];
  int chargeTimer;
  int power;
} formation3x1;

unit *newUnit (unitType t, char n[], char i, color c);
unit *newUnitFromProto (unitPrototype *up, color c);
void initUnit (unit *u, unitType t, char n[], char i, color c);
void freeUnit (unit *u);

bool cmpUnits (unit *u1, unit *u2);

void readUnits (unitPrototype *prototypeList, int ht);
void readWall (unitPrototype *protoWall, int *lvl1Wall, int *lvl2Wall,
               int *lvl3Wall, int ht);

void printUnit (unit *u);
#endif
