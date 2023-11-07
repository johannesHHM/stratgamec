#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <raylib.h>

// typedef enum unitAnimationStates
// {
//   idle = 0
// } unitAnimationStates;

// typedef struct UnitAnimationState
// {
//   unitAnimationStates state; // current state
//   int state_amount;          // max states
//   int frame;                 // current frame
//   int *frame_counts; // an entry for every state, an entry ex: [7, 5, 7, -1]
// } unitAnimationState;

// typedef struct UnitAnimationBank
// {
//   Texture2D ***textures;
// } unitAnimationBank;

// UnitAnimationBank *readUnitAnimationBank (int ht);

// Texture2D *getFrame (int ut, UnitAnimationState *state);
// typedef struct HeroUnitsAnimationDatabase // for 1 unit type
// {
// unitAnimationDatabase *unitAnimationDatabases; // animations[state][color]
// } heroUnitsAnimationDatabase;

#define UNIT_COUNT 3
#define UNIT_STATES 1

typedef enum unitAnimationState
{
  idle = 0
} unitAnimationState;

typedef struct Animation
{
  int spritesLen;
  Image *sprites;   // len of spritesLen, reference to all sprites in order
  int *frameCounts; // len of spritesLen, how long to display sprite
                    // ex: [7, 7, 5, 7]
} animation;

typedef struct UnitAnimationDatabase // for 1 unit type
{
  int animationUnitType;
  animation **animations; // animations[state][color]
} unitAnimationDatabase;

typedef struct HeroAnimationDatabase
{
  unitAnimationDatabase **unitAnimationDatabase; // is length of UNIT_COUNT
} heroAnimationDatabase;

void printHeroAnimationDatabase (heroAnimationDatabase *db);

heroAnimationDatabase *readHeroAnimationDatabase (int ht);

#endif
