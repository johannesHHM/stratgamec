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
                printf ("  %d: (%d,%d)\n", i, anim->sprites[i].width,
                        anim->sprites[i].height);
              }
            printf ("\n");
          }
    }
  printf ("\n");
}

heroAnimationDatabase *
readHeroAnimationDatabase (int ht)
{
  FILE *animationsFile;

  char *animationsFilePath;
  asprintf (&animationsFilePath, "data/hero/%d/animations/animationsData", ht);
  animationsFile = fopen (animationsFilePath, "r");

  if (animationsFile == NULL)
    {
      fprintf (stderr,
               "[ERROR] could not open animationsData file\n"
               "        file path: %s\n"
               "        hero type: %d\n",
               animationsFilePath, ht);
      free (animationsFilePath);

      return NULL;
    }

  free (animationsFilePath);

  int read = 0;

  int unitAmount = 0;
  read = fscanf (animationsFile, "%d\n", &unitAmount);
  if (read != 1)
    {
      fprintf (stderr,
               "[ERROR] could not parse unit count (first line, integer) from "
               "animations file\n"
               "        file path: data/hero/%d/animationsData\n"
               "        hero type: %d\n",
               ht, ht);
      fclose (animationsFile);

      return NULL;
    }

  heroAnimationDatabase *heroDatabase;
  heroDatabase = malloc (sizeof (heroAnimationDatabase));
  heroDatabase->unitAnimationDatabase
      = malloc (unitAmount * sizeof (unitAnimationDatabase));

  for (int unitCount = 0; unitCount < unitAmount; unitCount++)
    {
      int unitType;
      int animationAmount;
      char unitName[20];

      read = fscanf (animationsFile, "%d,%19[^,],%d\n", &unitType, unitName,
                     &animationAmount);

      if (read != 3)
        {
          fprintf (stderr,
                   "[ERROR] could not parse animationsData file\n"
                   "        file path: data/hero/%d/animationsData\n"
                   "        line number: %d\n"
                   "        hero type: %d\n",
                   ht, unitCount + 1, ht);

          return NULL;
        }

      unitAnimationDatabase *unitDatabase;
      unitDatabase = malloc (sizeof (unitAnimationDatabase));
      unitDatabase->unitType = unitType;
      unitDatabase->stateCount = animationAmount;
      unitDatabase->animations
          = malloc (animationAmount * sizeof (animation *));
      for (int i = 0; i < animationAmount; i++)
        unitDatabase->animations[i]
            = malloc (COLOR_COUNT * sizeof (animation *));

      FILE *animationDataFile;
      char *animationFilePath;
      asprintf (&animationFilePath, "data/hero/%d/animations/%s/animationData",
                ht, unitName);
      animationDataFile = fopen (animationFilePath, "r");

      if (animationDataFile == NULL)
        {
          fprintf (stderr,
                   "[ERROR] could not open animationData file\n"
                   "        file path: %s\n"
                   "        unit: %s\n"
                   "        hero type: %d\n",
                   animationFilePath, unitName, ht);
          free (animationFilePath);

          return NULL;
        }

      free (animationFilePath);

      char animationName[20];
      int animationFrameAmount;
      int animationOffX, animationOffY;

      for (int animationCount = 0; animationCount < animationAmount;
           animationCount++)
        {
          read
              = fscanf (animationDataFile, "%19[^:]:%d;%dx%d;", animationName,
                        &animationFrameAmount, &animationOffX, &animationOffY);

          if (read != 4)
            {
              fprintf (stderr,
                       "[ERROR] could not parse animationData file\n"
                       "        file path: "
                       "data/hero/%d/animations/%s/animationData\n"
                       "        hero type: %d\n",
                       ht, unitName, ht);

              return NULL;
            }

          int *frameCounts;
          frameCounts = malloc (animationFrameAmount * sizeof (int));
          for (int f = 0; f < animationFrameAmount; f++)
            {
              if (f < animationFrameAmount - 1)
                read = fscanf (animationDataFile, "%d,", &frameCounts[f]);
              else
                read = fscanf (animationDataFile, "%d\n", &frameCounts[f]);

              if (read != 1)
                {
                  fprintf (stderr,
                           "[ERROR] could not parse animationData file\n"
                           "        file path: "
                           "data/hero/%d/animations/%s/animationData\n"
                           "        unit: %s\n"
                           "        hero type: %d\n",
                           ht, unitName, unitName, ht);

                  return NULL;
                }
            }

          for (int c = 0; c < COLOR_COUNT; c++)
            {
              animation *currentAnim;
              currentAnim = malloc (sizeof (animation));
              currentAnim->spritesLen = animationFrameAmount;
              currentAnim->offX = animationOffX;
              currentAnim->offY = animationOffY;

              currentAnim->frameCounts
                  = malloc (currentAnim->spritesLen * sizeof (int));
              memcpy (currentAnim->frameCounts, frameCounts,
                      currentAnim->spritesLen * sizeof (int));

              currentAnim->sprites
                  = malloc (currentAnim->spritesLen * sizeof (Texture2D));

              for (int f = 0; f < currentAnim->spritesLen; f++)
                {
                  char *texturePath;
                  asprintf (&texturePath,
                            "data/hero/%d/animations/%s/%s/%d_%d.png", ht,
                            unitName, animationName, c, f);
                  currentAnim->sprites[f] = LoadTexture (texturePath);
                  if (currentAnim->sprites[f].id <= 0)
                    {
                      fprintf (
                          stderr,
                          "[ERROR] could not load texture (in animation)\n"
                          "        file path: %s\n"
                          "        unit: %s\n"
                          "        hero type: %d\n",
                          texturePath, unitName, ht);

                      return NULL;
                    }
                  free (texturePath);
                }
              unitDatabase->animations[animationCount][c] = currentAnim;
            }
          free (frameCounts);
        }
      heroDatabase->unitAnimationDatabase[unitCount] = unitDatabase;
    }
  fclose (animationsFile);

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
