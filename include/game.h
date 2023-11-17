#ifndef _GAME_H_
#define _GAME_H_

#include "raylib.h"

#include <stdlib.h>

typedef struct Game
{
  int screenWidth;
  int screenHeight;

  int seed;
} game;

game initGame ();

void runGame ();

#endif