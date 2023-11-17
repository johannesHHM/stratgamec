#include "sprites.h"
#include "colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

void
printHeroAnimationDatabase (heroAnimationDatabase *db)
{
  for (int i = 0; i < UNIT_ANIMATION_COUNT; i++)
    {
      printf ("unitType: %d, %d states\n",
              db->unitAnimationDatabase[i]->unitType,
              db->unitAnimationDatabase[i]->stateCount);
      for (int s = 0; s < db->unitAnimationDatabase[i]->stateCount; s++)
        for (int c = 0; c < COLOR_COUNT; c++)
          {
            animation *anim;
            anim = db->unitAnimationDatabase[i]->animations[s][c];
            printf ("spritesLen: %d\n", anim->spritesLen);
            printf ("frameCounts: [ ");
            for (int i = 0; i < anim->spritesLen; i++)
              {
                if (i == anim->spritesLen - 1)
                  printf ("%d ", anim->frameCounts[i]);
                else
                  printf ("%d, ", anim->frameCounts[i]);
              }
            printf ("]\n");
            printf ("sprites:\n");
            for (int i = 0; i < anim->spritesLen; i++)
              {
                printf ("%d: (%d,%d)\n", i, anim->sprites[i].width,
                        anim->sprites[i].height);
              }
          }
    }
  printf ("\n");
}

// TODO fix whatever the fuck this function is
// TODO how to free all this shit, who to free all this shit
heroAnimationDatabase *
readHeroAnimationDatabase (int ht)
{
  FILE *animationsFile;

  char *filename;
  asprintf (&filename, "data/hero/%d/animations", ht);
  animationsFile = fopen (filename, "r");
  free (filename);

  if (animationsFile == NULL)
    {
      printf ("Error opening animations file.\n");
    }

  int read = 0;
  // int records = 0;

  int animationCountOLD = -1;
  read = fscanf (animationsFile, "%d\n", &animationCountOLD);
  if (read != 1)
    printf ("Error could not read animation count.\n");

  heroAnimationDatabase *heroDatabase;
  heroDatabase = malloc (sizeof (heroAnimationDatabase));
  heroDatabase->unitAnimationDatabase
      = malloc (animationCountOLD * sizeof (unitAnimationDatabase));

  int unitCount = 0;
  for (int _ = 0; _ < animationCountOLD; _++)
    {
      int ut;
      int animationCount;
      read = fscanf (animationsFile, "%d,%d\n", &ut, &animationCount);

      // printf ("ut: %d, ac: %d\n", ut, animationCount);

      unitAnimationDatabase *uAniDb;
      uAniDb = malloc (sizeof (unitAnimationDatabase));
      uAniDb->unitType = ut;
      uAniDb->stateCount = animationCount;
      uAniDb->animations = malloc (animationCount * sizeof (animation *));
      for (int i = 0; i < animationCount; i++)
        uAniDb->animations[i] = malloc (COLOR_COUNT * sizeof (animation *));

      for (int anim = 0; anim < animationCount; anim++)
        {
          FILE *animationFile;
          char *filename;
          asprintf (&filename, "data/hero/%d/sprites/%d/%d/animation", ht, ut,
                    anim);
          animationFile = fopen (filename, "r");
          free (filename);

          int spritesLength = 0;
          int *frameCounts;
          read = fscanf (animationFile, "%d;", &spritesLength);

          frameCounts = malloc (spritesLength * sizeof (int));
          for (int f = 0; f < spritesLength; f++)
            {
              read = fscanf (animationFile, "%d,", &frameCounts[f]);
            }

          // printf ("read: %d\n", read);

          for (int c = 0; c < COLOR_COUNT; c++)
            {
              animation *currentAnim;
              currentAnim = malloc (sizeof (animation));
              currentAnim->spritesLen = spritesLength;

              currentAnim->frameCounts
                  = malloc (currentAnim->spritesLen * sizeof (int));
              memcpy (currentAnim->frameCounts, frameCounts,
                      currentAnim->spritesLen * sizeof (int));

              // printf ("IN: spritesLen: %d\n", currentAnim->spritesLen);

              // printf ("spritesLen: %d\n", currAnim.spritesLen);

              currentAnim->sprites
                  = malloc (currentAnim->spritesLen * sizeof (Image));

              for (int f = 0; f < currentAnim->spritesLen; f++)
                {
                  char *filename;
                  asprintf (&filename, "data/hero/%d/sprites/%d/%d/%d_%d.png",
                            ht, ut, anim, f, c);
                  currentAnim->sprites[f] = LoadTexture (filename);
                  free (filename);
                }
              uAniDb->animations[anim][c] = currentAnim;
            }
          free (frameCounts);
        }
      heroDatabase->unitAnimationDatabase[unitCount] = uAniDb;
      unitCount++;
    }

  return heroDatabase;
}

void
freeHeroAnimationDatabase (heroAnimationDatabase *db)
{
  for (int i = 0; i < UNIT_ANIMATION_COUNT; i++)
    {
      unitAnimationDatabase *unitDatabase;
      unitDatabase = db->unitAnimationDatabase[i];
      for (int s = 0; s < unitDatabase->stateCount; s++)
        for (int c = 0; c < COLOR_COUNT; c++)
          {
            animation *anim;
            anim = unitDatabase->animations[s][c];
            for (int i = 0; i < anim->spritesLen; i++)
              UnloadTexture (anim->sprites[i]);

            free (anim->sprites);
            free (anim->frameCounts);
            free (anim);
          }

      for (int i = 0; i < unitDatabase->stateCount; i++)
        free (unitDatabase->animations[i]);

      free (unitDatabase->animations);
      free (unitDatabase);
    }
  free (db->unitAnimationDatabase);
  free (db);
  db = NULL;
}

unitAnimationDatabase *
matchUnitToDatabase (int ut, heroAnimationDatabase *db)
{
  for (int i = 0; i < UNIT_ANIMATION_COUNT; i++)
    if (ut == db->unitAnimationDatabase[i]->unitType)
      return db->unitAnimationDatabase[i];

  fprintf (stderr,
           "[ERROR] could not find unit animation in hero "
           "database\n        unit type: %d\n",
           ut);
  return NULL;
}
