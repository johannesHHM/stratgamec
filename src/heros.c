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
  readUnits (h->protoList, hc);
  readWall (&h->protoWall, &h->level1Wall, &h->level2Wall, &h->level3Wall, hc);
  return h;
}

void
freeHero (hero *h)
{
  free (h);
  h = NULL;
}

void
printHero (hero *h)
{
  printf ("%s - %d\n%s: %d, %d, %d\n", h->name, (int)h->type, h->protoWall.name, h->level1Wall,
          h->level2Wall, h->level3Wall);
}
