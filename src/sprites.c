#include "sprites.h"
#include "colors.h"

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

void
printHeroAnimationDatabase (heroAnimationDatabase *db)
{
  // printf ("unitType: %d\n", db->animationUnitType);
  printf ("\n");
}

// TODO fix whatever the fuck this function is
heroAnimationDatabase *
readHeroAnimationDatabase (int ht)
{
  FILE *animationsFile;

  char *filename;
  asprintf (&filename, "data/hero/%d/sprites/animations", ht);
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

  do
    {
      int unitCount = 0;
      int ut;
      int animationCount;
      read = fscanf (animationsFile, "%d,%d\n", &ut, &animationCount);

      printf ("ut: %d, ac: %d\n", ut, animationCount);

      unitAnimationDatabase *uAniDb;
      uAniDb = malloc (sizeof (unitAnimationDatabase));
      uAniDb->animationUnitType = ut;
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

          for (int c = 0; c < COLOR_COUNT; c++)
            {
              animation currAnim;
              fscanf (animationFile, "%d;", &currAnim.spritesLen);

              printf ("spritesLen: %d\n", currAnim.spritesLen);

              currAnim.sprites = malloc (currAnim.spritesLen * sizeof (Image));
              currAnim.frameCounts
                  = malloc (currAnim.spritesLen * sizeof (int));
              for (int f = 0; f < currAnim.spritesLen; f++)
                {
                  fscanf (animationFile, "%d,", &currAnim.frameCounts[f]);
                  char *filename2;
                  asprintf (&filename2, "data/hero/%d/sprites/%d/%d/%d_%d.png",
                            ht, ut, anim, f, c);
                  currAnim.sprites[f] = LoadImage (filename2);
                  free (filename2);
                }
              uAniDb->animations[anim][c] = currAnim;
            }
        }
      heroDatabase->unitAnimationDatabase[unitCount] = uAniDb;
      unitCount++;
    }
  while (!feof (animationsFile));

  return heroDatabase;
}