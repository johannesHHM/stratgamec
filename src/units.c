#include "units.h"
#include "heros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unit *
newUnit (unitType t, char n[], char i, color c)
{
  unit *u = malloc (sizeof (unit));
  u->type = t;
  strcpy (u->name, n);
  u->icon = i;
  u->color = c;
  u->taggedWall = false;
  u->taggedAttack = false;
  return u;
}

unit *
newUnitFromProto (unitPrototype *up, color c)
{
  unit *u = malloc (sizeof (unit));
  u->type = up->type;
  strcpy (u->name, up->name);
  u->icon = up->icon;
  u->color = c;
  u->taggedWall = false;
  u->taggedAttack = false;
  return u;
}

void
initUnit (unit *u, unitType t, char n[], char i, color c)
{
  u->type = t;
  strcpy (u->name, n);
  u->icon = i;
  u->color = c;
  u->taggedWall = false;
  u->taggedAttack = false;
}

void
freeUnit (unit *u)
{
  free (u);
  u = NULL;
}

bool
cmpUnits (unit *u1, unit *u2)
{
  if (!u1 || !u2)
    return false;
  return ((u1->type == u2->type) && (strcmp (u1->name, u2->name) == 0)
          && (u1->color == u2->color));
}

void
readUnits (unitPrototype *prototypeList, int ht)
{
  FILE *file;

  char *filename;
  asprintf (&filename, "data/units/%d/units", ht);
  file = fopen (filename, "r");
  free (filename);

  if (file == NULL)
    {
      printf ("Error opening file.\n");
    }

  int read = 0;
  int records = 0;
  do
    {
      read = fscanf (file, "%d,%19[^,],%c,%d\n",
                     (int *)&prototypeList[records].type,
                     prototypeList[records].name, &prototypeList[records].icon,
                     &prototypeList[records].strength);
      if (read == 4)
        records++;
      if (read != 4 && !feof (file))
        {
          printf ("File format incorrect.\n");
        }

      if (ferror (file))
        {
          printf ("Error reading file.\n");
        }
    }
  while (!feof (file));

  fclose (file);
}
