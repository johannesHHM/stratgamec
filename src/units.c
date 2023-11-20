#include "units.h"
#include "sprites.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO set strength
unit *
newUnit (unitType t, char n[], char i, colorG c)
{
  unit *u = malloc (sizeof (unit));
  u->type = t;
  strcpy (u->name, n);
  u->icon = i;
  u->tagIcon = 'n';
  u->color = c;
  u->taggedWall = false;
  u->taggedAttack = false;
  u->hasFormation = false;
  u->firstFormation = false;

  return u;
}

unit *
newUnitFromProto (unitPrototype *up, colorG c)
{
  unit *u = malloc (sizeof (unit));
  u->occupied = true;
  u->type = up->type;
  strcpy (u->name, up->name);
  u->icon = up->icon;
  u->tagIcon = 'n';
  u->color = c;
  u->strength = up->strength;
  u->taggedWall = false;
  u->taggedAttack = false;
  u->hasFormation = false;
  u->firstFormation = false;

  return u;
}

unit
initUnitFromProto (unitPrototype *up, colorG c, heroAnimationDatabase *hdb)
{
  unit u;
  u.occupied = true;
  u.type = up->type;
  strcpy (u.name, up->name);
  u.icon = up->icon;
  u.tagIcon = 'n';
  u.color = c;
  u.strength = up->strength;
  u.taggedWall = false;
  u.taggedAttack = false;
  u.hasFormation = false;
  u.firstFormation = false;
  u.animationDb = matchUnitToDatabase ((int)u.type, hdb);

  u.animData.frameCount = 0;
  u.animData.sprite = 0;
  u.animData.state = idle;

  return u;
}

// TODO set strength
void
initUnit (unit *u, unitType t, char n[], char i, colorG c)
{
  u->type = t;
  strcpy (u->name, n);
  u->icon = i;
  u->tagIcon = 'n';
  u->color = c;
  u->taggedWall = false;
  u->taggedAttack = false;
  u->hasFormation = false;
  u->firstFormation = false;
}

formation3x1 *
newFormationFromProto3x1 (formationPrototype3x1 *fp, colorG c)
{
  formation3x1 *f = malloc (sizeof (formation3x1));
  f->type = fp->type;
  f->chargeTimer = fp->chargeTimer;
  f->power = fp->power;

  return f;
}

void
freeUnit (unit *u)
{
  freeFormation3x1 (u->formation3x1);
  free (u);
  u = NULL;
}

void
freeFormation3x1 (formation3x1 *f)
{
  free (f);
  f = NULL;
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
  asprintf (&filename, "data/hero/%d/units", ht);
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

void
readFormations3x1 (formationPrototype3x1 *prototypeList, int ht)
{
  FILE *file;

  char *filename;
  asprintf (&filename, "data/hero/%d/formations3x1", ht);
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
      read = fscanf (file, "%d,%d,%d\n", (int *)&prototypeList[records].type,
                     (int *)&prototypeList[records].chargeTimer,
                     (int *)&prototypeList[records].power);
      if (read == 3)
        records++;

      if (read != 3 && !feof (file))
        {
          printf ("File format incorrect.\n");
          printf ("%d  %d\n", ht, records);
          printf ("type: %d, charge: %d, power: %d\n",
                  prototypeList[records].type,
                  prototypeList[records].chargeTimer,
                  prototypeList[records].power);
        }

      if (ferror (file))
        {
          printf ("Error reading file.\n");
        }
    }
  while (!feof (file));

  fclose (file);
}

void
readWall (unitPrototype *protoWall, int *lvl1Wall, int *lvl2Wall,
          int *lvl3Wall, int ht)
{
  FILE *file;

  char *filename;
  asprintf (&filename, "data/hero/%d/wall", ht);
  file = fopen (filename, "r");
  free (filename);

  if (file == NULL)
    {
      printf ("Error opening file.\n");
    }

  int read = 0;

  read = fscanf (file, "%d,%19[^,],%c,%d,%d,%d", (int *)&protoWall->type,
                 protoWall->name, &protoWall->icon, lvl1Wall, lvl2Wall,
                 lvl3Wall);

  protoWall->strength = *lvl1Wall;

  if (read != 6 && !feof (file))
    {
      printf ("File format incorrect.\n");
    }

  if (ferror (file))
    {
      printf ("Error reading file.\n");
    }

  fclose (file);
}

void
tickUnitAnimationData (unit *u)
{
  int frameLength, spritesLength;
  frameLength = u->animationDb->animations[u->animData.state][u->color]
                    ->frameCounts[u->animData.sprite];
  spritesLength
      = u->animationDb->animations[u->animData.state][u->color]->spritesLen;

  u->animData.frameCount = (u->animData.frameCount + 1) % frameLength;
  if (u->animData.frameCount == 0)
    {
      u->animData.sprite = (u->animData.sprite + 1) % spritesLength;
    }
}

Texture2D *
getUnitTexture (unit *u)
{
  return &u->animationDb->animations[u->animData.state][u->color]
              ->sprites[u->animData.sprite];
}

bool
canBeMoved (unit *u)
{
  if ((int)u->type < 10)
    return false;
  if (u->hasFormation)
    return false;
  return true;
}
bool
canBeRemoved (unit *u)
{
  if (u->hasFormation)
    return false;
  return true;
}

void
printUnit (unit *u)
{
  printf ("Unit: %s\ni: %c c: %d\ntagWall: %d tagAtc: %d hasFrm: %d\n",
          u->name, u->icon, u->color, u->taggedWall, u->taggedAttack,
          u->hasFormation);
}
