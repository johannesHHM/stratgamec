#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <raylib.h>

#define UNIT_ANIMATION_COUNT 4
#define UNIT_STATES 1

typedef enum AnimationState
{
  idle = 0,
  walking = 1,
  poof = 2
} unitAnimState;

typedef struct UnitAnimationStateData
{
  int sprite;     // index of sprite in animation
  int frameCount; // count of frames
  unitAnimState state;

  int offX, offY;
  bool lockedAnimating;
  bool executeEnd;
} unitAnimData;

typedef struct Animation
{
  int spritesLen;
  Texture2D *sprites; // len of spritesLen, reference to all sprites in order
  int *frameCounts;   // len of spritesLen, how long to display sprite
                      // ex: [7, 7, 5, 7]
} animation;

typedef struct UnitAnimationDatabase // for 1 unit type
{
  int unitType;
  int stateCount;
  animation ***animations; // animations[state][color]
} unitAnimationDatabase;

typedef struct HeroAnimationDatabase
{
  unitAnimationDatabase **unitAnimationDatabase; // is length of UNIT_COUNT
} heroAnimationDatabase;

void printHeroAnimationDatabase (heroAnimationDatabase *db);
heroAnimationDatabase *readHeroAnimationDatabase (int ht);
void freeHeroAnimationDatabase (heroAnimationDatabase *db);

unitAnimationDatabase *matchUnitToDatabase (int ut, heroAnimationDatabase *db);

#endif
