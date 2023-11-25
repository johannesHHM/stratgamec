#include "heros.h"
#include "units.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hero *
newHero (heroType hc, char *n, int maxUnits)
{
  hero *h = malloc (sizeof (hero));
  h->type = hc;
  strcpy (h->name, n);
  readUnits (h->unitProtoList, hc);
  readFormations3x1 (h->formationProtoList, hc);
  readWall (&h->protoWall, &h->level1Wall, &h->level2Wall, &h->level3Wall, hc);
  h->maxUnits = maxUnits;

  h->activeUnits[0] = h->unitProtoList[2];
  h->activeUnits[1] = h->unitProtoList[0];
  h->activeUnits[2] = h->unitProtoList[1];

  h->animationDb = readHeroAnimationDatabase (hc);
  return h;
}

void
freeHero (hero *h)
{
  freeHeroAnimationDatabase (h->animationDb);
  free (h);
  h = NULL;
}

formationPrototype3x1 *
getFormationPrototype (hero *hero, unitType type)
{
  for (int i = 0; i < UNIT_COUNT; i++)
    if (hero->formationProtoList[i].type == type)
      return &hero->formationProtoList[i];
  return NULL;
}

unit
getRandomUnit (hero *h)
{
  int r = rand () % 3;
  return initUnitFromProto (&h->activeUnits[r], (colorG)r, h->animationDb);
}

void
printHero (hero *h)
{
  printf ("%s - %d\n%s: %d, %d, %d\n", h->name, (int)h->type,
          h->protoWall.name, h->level1Wall, h->level2Wall, h->level3Wall);
}
