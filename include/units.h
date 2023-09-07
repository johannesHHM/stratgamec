#ifndef _UNITS_H_
#define _UNITS_H_

#include "colors.h"
#include <stdbool.h>

typedef enum unitType {
    spearman = 011,
    swordman = 012,
    archer = 013,
    copter = 111,
    engineer = 112,
    bombling = 113
} unitType;

typedef struct Unit {
    unitType type;
    char name[20];
    char icon;
    color color;

    int strength;

    bool taggedWall;
    bool taggedAttck;
} unit;

typedef struct UnitPrototype {
    unitType type;
    char name[20];
    char icon;
    int strength;
} unitPrototype;

unit* newUnit(unitType t, char n[], char i, color c);

unit* newUnitFromProto(unitPrototype* up, color c);

void initUnit(unit* u, unitType t, char n[], char i, color c);

void freeUnit(unit* u);

void readUnits(unitPrototype* prototypeList, int ht);

#endif


