#include "game.h"
#include "heros.h"
#include "match.h"
#include "units.h"

#include "raylib.h"

#include <stdio.h>
#include <time.h>

void
runGame ()
{

  const int screenWidth = 804;
  const int screenHeight = 604;

  const int virtualScreenWidth = 201;
  const int virtualScreenHeight = 151;

  SetTraceLogLevel (LOG_ERROR);
  InitWindow (screenWidth, screenHeight, "temp_window_title");
  SetTargetFPS (60);

  int seed = time (NULL);
  // seed = 1697559963;
  // seed = 1699665000;
  printf ("\nRunning Main\n\n");
  printf ("seed: %d\n", seed);
  srand (seed);

  const float virtualRatio = (float)screenWidth / (float)virtualScreenWidth;

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
      = { -virtualRatio, -virtualRatio, screenWidth + (virtualRatio * 2),
          screenHeight + (virtualRatio * 2) };

  Vector2 origin = { 0.0f, 0.0f };

  float cameraX = 0.0f;
  float cameraY = 0.0f;

  hero *hero1 = newHero (paladin, "Uther", 25);
  hero *hero2 = newHero (paladin, "Jaina", 25);

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
                        DrawTexture (text, 1 + (y * 25), 1 + (x * 25), WHITE);
                      else
                        DrawTexture (text, 1 + (y * 25), 1 + (x * 25),
                                     (Color){ 120, 120, 120, 255 });
                    }
                  else
                    DrawTexture (temp, 1 + (y * 25), 1 + (x * 25), WHITE);
                }
            }
        }

      DrawTexture (board, 0, 0, WHITE);

      EndMode2D ();
      EndTextureMode ();

      BeginDrawing ();
      ClearBackground (RED);

      BeginMode2D (screenSpaceCamera);
      DrawTexturePro (target.texture, sourceRec, destRec, origin, 0.0f, WHITE);
      EndMode2D ();

      // DrawText (
      //     TextFormat ("Screen resolution: %ix%i", screenWidth,
      //     screenHeight), 10, 10, 20, DARKBLUE);
      // DrawText (TextFormat ("World resolution: %ix%i",
      virtualScreenWidth,
          //                       virtualScreenHeight),
          //           10, 40, 20, DARKGREEN);
          DrawFPS (GetScreenWidth () - 95, 10);
      EndDrawing ();

      // // DRAW
      // BeginDrawing ();
      // ClearBackground (RAYWHITE);

      // if (match->board1->board[0][0].occupied)
      //   {
      //     Texture2D text
      //         = LoadTextureFromImage (match->board1->board[0][0]
      //                                     .animationDb->animations[0][0]
      //                                     ->sprites[0]);
      //     DrawTexture (text, 10, 10, WHITE);
      //   }
      // DrawText ("Congrats! You created your first window!", 190, 200,
      // 20,
      //           BLACK);
      // EndDrawing ();
    }

  freeMatch (match);

  CloseWindow ();

  // test ();

  exit (0);
}
