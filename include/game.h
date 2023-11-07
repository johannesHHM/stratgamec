#ifndef _GAME_H_
#define _GAME_H_

#include "raylib.h"

#include <stdlib.h>

typedef struct SpriteBank
{
  Texture2D *sprites;
} spriteBank;

void runGame ();

#endif