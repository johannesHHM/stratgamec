#ifndef _UNITS_H_
#define _UNITS_H_

#include "colors.h"
#include "sprites.h"

#include <stdbool.h>

typedef struct Formation3x1 formation3x1;

typedef enum unitType
{
  paladinWall = 1,
  mechanicWall = 2,
  wizardWall = 3,

  archer = 011,
  spearman = 012,
  swordman = 013,
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
  colorG color;

  int strength;

  bool taggedWall;
  bool taggedAttack;
  bool hasFormation;
  bool firstFormation;

  formation3x1 *formation3x1;

  unitAnimationDatabase *animationDb;
  unitAnimData animData;
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

unit *newUnit (unitType t, char n[], char i, colorG c);
unit *newUnitFromProto (unitPrototype *up, colorG c);
unit initUnitFromProto (unitPrototype *up, colorG c,
                        heroAnimationDatabase *hdb);
void initUnit (unit *u, unitType t, char n[], char i, colorG c);

void freeUnit (unit *u);
void freeFormation3x1 (formation3x1 *f);

bool cmpUnits (unit *u1, unit *u2);

void readUnits (unitPrototype *prototypeList, int ht);
void readWall (unitPrototype *protoWall, int *lvl1Wall, int *lvl2Wall,
               int *lvl3Wall, int ht);

void readFormations3x1 (formationPrototype3x1 *prototypeList, int ht);

formation3x1 *newFormationFromProto3x1 (formationPrototype3x1 *fp, colorG c);
void freeFormation3x1 (formation3x1 *f);

void tickUnitAnimationData (unit *u);
Texture2D *getUnitTexture (unit *u);

void printUnit (unit *u);

#endif
