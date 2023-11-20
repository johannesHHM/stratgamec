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
  g.screenWidth = 265 * 4;
  g.screenHeight = 199 * 4;

  g.seed = time (NULL);
  // g.seed = 1700452227;

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

  hero *hero1 = newHero (paladin, "hero1", 30);
  hero *hero2 = newHero (paladin, "hero2", 30);

  match *match;
  match = newMatch (hero1, hero2);

  // GAME LOOP
  while (!WindowShouldClose ())
    {
      printf ("seed: %d\n", g.seed);

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

      // Draw cursor
      DrawRectangle (match->cursorPosition.y * unitSize + 1,
                     match->cursorPosition.x * unitSize + 1, 32, 32,
                     (Color){ 255, 213, 0, 200 });

      // Draw hover guy
      if (match->hasUnitSelected)
        {
          if (match->selectedUnit.animationDb)
            {
              Texture2D *text = getUnitTexture (&match->selectedUnit);
              DrawTexture (*text, 1 + (match->selectedPosition.y * unitSize),
                           1 + (match->selectedPosition.x * unitSize),
                           (Color){ 255, 255, 255, 100 });
            }
          else
            {
              DrawTexture (temp, 1 + (match->selectedPosition.y * unitSize),
                           1 + (match->selectedPosition.x * unitSize), WHITE);
            }
        }

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
                      tickUnitAnimationData (u);
                      Texture2D *text = getUnitTexture (u);

                      if (!u->hasFormation)
                        DrawTexture (*text, 1 + (y * unitSize),
                                     1 + (x * unitSize), WHITE);
                      else
                        DrawTexture (*text, 1 + (y * unitSize),
                                     1 + (x * unitSize),
                                     (Color){ 255, 255, 255, 120 });
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
      // ClearBackground (RED);

      BeginMode2D (screenSpaceCamera);
      DrawTexturePro (target.texture, sourceRec, destRec, origin, 0.0f, WHITE);

      EndMode2D ();

      if (match->debug)
        {
          DrawRectangle (5, 5, 260, 100, (Color){ 255, 255, 255, 200 });

          DrawText (TextFormat ("FPS: %d", GetFPS ()), 10, 10, 20, BLACK);

          DrawText (TextFormat ("pointSelected: (%d, %d)",
                                match->cursorPosition.x,
                                match->cursorPosition.y),
                    10, 30, 20, BLACK);
          if (match->hasUnitSelected)
            DrawText (
                TextFormat ("unitSelected: %s", match->selectedUnit.name), 10,
                50, 20, BLACK);
          else
            DrawText ("unitSelected: None", 10, 50, 20, BLACK);

          DrawText (TextFormat ("backupUnits: %d", match->board1->backupUnits),
                    10, 70, 20, BLACK);
        }

      EndDrawing ();
    }

  freeMatch (match);
  freeHero (hero1);
  freeHero (hero2);

  CloseWindow ();

  // test ();

  exit (0);
}
