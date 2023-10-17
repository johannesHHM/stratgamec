#include "heros.h"
#include "units.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hero *
newHero (heroType hc, char *n)
{
  hero *h = malloc (sizeof (hero));
  h->type = hc;
  strcpy (h->name, n);
  readUnits (h->unitProtoList, hc);
  readFormations3x1 (h->formationProtoList, hc);
  readWall (&h->protoWall, &h->level1Wall, &h->level2Wall, &h->level3Wall, hc);
  return h;
}

void
freeHero (hero *h)
{
  free (h);
  h = NULL;
}

formationPrototype3x1 *
getFormationPrototype (hero *hero, unitType type)
{
  for (int i = 0; i < UNIT_COUNT; i++)
    {
      if (hero->formationProtoList[i].type == type)
        {
          return &hero->formationProtoList[i];
        }
    }
}

void
printHero (hero *h)
{
  printf ("%s - %d\n%s: %d, %d, %d\n", h->name, (int)h->type,
          h->protoWall.name, h->level1Wall, h->level2Wall, h->level3Wall);
}
