#include "game.h"
#include "heros.h"
#include "match.h"

#include "raylib.h"

#include <stdio.h>
#include <time.h>

void
runGame ()
{
  int seed = time (NULL);
  // seed = 1697559963;
  printf ("\nRunning Main\n\n");
  printf ("seed: %d\n", seed);
  srand (seed);

  const int screenWidth = 800;
  const int screenHeight = 800;

  SetTraceLogLevel (LOG_ERROR);
  InitWindow (screenWidth, screenHeight, "temp_window_title");
  SetTargetFPS (60);

  hero *hero1 = newHero (paladin, "Uther", 25);
  hero *hero2 = newHero (wizard, "Jaina", 25);

  match *match;
  match = newMatch (hero1, hero2);

  // GAME LOOP
  while (!WindowShouldClose ())
    {

      runMatch (match);

      // DRAW
      BeginDrawing ();
      ClearBackground (RAYWHITE);
      DrawText ("Congrats! You created your first window!", 190, 200, 20,
                BLACK);
      EndDrawing ();
    }

  freeMatch (match);

  CloseWindow ();

  // test ();

  exit (0);
}
