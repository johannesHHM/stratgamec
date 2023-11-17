#include "game.h"
#include "heros.h"
#include "match.h"
#include "units.h"

#include "raylib.h"

#include <stdio.h>
#include <time.h>

game
initGame ()
{
  game g;
  g.screenWidth = 265 * 3;
  g.screenHeight = 199 * 3;

  g.seed = time (NULL);

  SetTraceLogLevel (LOG_ERROR);
  InitWindow (g.screenWidth, g.screenHeight, "temp_window_title");
  SetTargetFPS (60);

  srand (g.seed);

  return g;
}

void
runGame ()
{
  game g = initGame ();

  const int virtualScreenWidth = 265;
  const int virtualScreenHeight = 199;

  const int unitSize = 33;

  printf ("\nRunning Main\n\n");
  printf ("seed: %d\n", g.seed);

  const float virtualRatio = (float)g.screenWidth / (float)virtualScreenWidth;

  Camera2D worldSpaceCamera = { 0 }; // Game world camera
  worldSpaceCamera.zoom = 1.0f;

  Camera2D screenSpaceCamera = { 0 }; // Smoothing camera
  screenSpaceCamera.zoom = 1.0f;

  RenderTexture2D target
      = LoadRenderTexture (virtualScreenWidth, virtualScreenHeight);

  Texture2D board = LoadTexture ("data/board-outline.png");
  Texture2D temp = LoadTexture ("data/temp-missing-unit.png");

  printf ("HELLO!\n");
  fflush (stdout);

  Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width,
                          -(float)target.texture.height };
  Rectangle destRec
      = { -virtualRatio, -virtualRatio, g.screenWidth + (virtualRatio * 2),
          g.screenHeight + (virtualRatio * 2) };

  Vector2 origin = { 0.0f, 0.0f };

  float cameraX = 0.0f;
  float cameraY = 0.0f;

  hero *hero1 = newHero (paladin, "hero1", 25);
  hero *hero2 = newHero (paladin, "hero2", 25);

  match *match;
  match = newMatch (hero1, hero2);

  // GAME LOOP
  while (!WindowShouldClose ())
    {

      runMatch (match);

      // Set the camera's target to the values computed above
      screenSpaceCamera.target = (Vector2){ cameraX, cameraY };

      // Round worldSpace coordinates, keep decimals into screenSpace
      // coordinates
      worldSpaceCamera.target.x = (int)screenSpaceCamera.target.x;
      screenSpaceCamera.target.x -= worldSpaceCamera.target.x;
      screenSpaceCamera.target.x *= virtualRatio;

      worldSpaceCamera.target.y = (int)screenSpaceCamera.target.y;
      screenSpaceCamera.target.y -= worldSpaceCamera.target.y;
      screenSpaceCamera.target.y *= virtualRatio;
      //----------------------------------------------------------------------------------
      // Draw
      //----------------------------------------------------------------------------------
      BeginTextureMode (target);
      ClearBackground (RAYWHITE);

      BeginMode2D (worldSpaceCamera);

      // Board background
      DrawTexture (board, 0, 0, WHITE);

      // Draw board units
      for (int x = 0; x < 6; ++x)
        {
          for (int y = 0; y < 8; ++y)
            {
              if (match->board1->board[x][y].occupied)
                {
                  unit *u = &match->board1->board[x][y];
                  if (u->animationDb)
                    {
                      Texture2D text
                          = u->animationDb->animations[0][(int)u->color]
                                ->sprites[0];
                      if (!u->hasFormation)
                        DrawTexture (text, 1 + (y * unitSize),
                                     1 + (x * unitSize), WHITE);
                      else
                        DrawTexture (text, 1 + (y * unitSize),
                                     1 + (x * unitSize),
                                     (Color){ 120, 120, 120, 255 });
                    }
                  else
                    DrawTexture (temp, 1 + (y * unitSize), 1 + (x * unitSize),
                                 WHITE);
                }
            }
        }

      EndMode2D ();
      EndTextureMode ();

      BeginDrawing ();
      ClearBackground (RED);

      BeginMode2D (screenSpaceCamera);
      DrawTexturePro (target.texture, sourceRec, destRec, origin, 0.0f, WHITE);
      EndMode2D ();

      DrawFPS (GetScreenWidth () - 95, 10);
      EndDrawing ();
    }

  freeMatch (match);
  freeHero (hero1);
  freeHero (hero2);

  CloseWindow ();

  // test ();

  exit (0);
}
