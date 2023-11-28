#include "game.h"
#include "heros.h"
#include "match.h"
#include "units.h"

#include "raylib.h"

#include <stdio.h>
#include <time.h>

#define ZOOM 5

game
initGame ()
{
  game g;
  g.screenWidth = UNIT_SIZE * HIGHT_C * ZOOM;
  g.screenHeight = UNIT_SIZE * WIDTH_C * ZOOM;

  g.seed = time (NULL);
  // g.seed = 1700843513;

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

  const int virtualScreenWidth = UNIT_SIZE * HIGHT_C + 4;
  const int virtualScreenHeight = UNIT_SIZE * WIDTH_C + 4;

  const int offset[2] = { 1, 1 };

  const int unitSize = 20;

  printf ("seed: %d\n", g.seed);

  const float virtualRatio = (float)g.screenWidth / (float)virtualScreenWidth;

  Camera2D worldSpaceCamera = { 0 }; // Game world camera
  worldSpaceCamera.zoom = 1.0f;

  Camera2D screenSpaceCamera = { 0 }; // Smoothing camera
  screenSpaceCamera.zoom = 1.0f;

  Color brightness = { 230, 230, 230, 255 };

  RenderTexture2D target
      = LoadRenderTexture (virtualScreenWidth, virtualScreenHeight);

  Texture2D trees = LoadTexture ("data/trees.png");
  Texture2D board = LoadTexture ("data/board-outline.png");
  Texture2D temp = LoadTexture ("data/temp-missing-unit.png");

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
      if (IsKeyPressed (KEY_P))
        {
          printf ("seed: %d\n", g.seed);
          printBoard (match->board1);
        }

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
      DrawTexture (board, 0, 0, (Color){ 215, 215, 215, 255 });

      // Draw cursor
      DrawRectangle (match->cursorPosition.y * unitSize + 1 + offset[0],
                     match->cursorPosition.x * unitSize + 1 + offset[1],
                     UNIT_SIZE, UNIT_SIZE, (Color){ 255, 213, 0, 200 });

      // Draw hover guy
      if (match->hasUnitSelected)
        {
          if (match->selectedUnit.animationDb)
            {
              Texture2D *text = getUnitTexture (&match->selectedUnit);
              DrawTexture (
                  *text,
                  1 + (match->selectedPosition.y * unitSize) + offset[0],
                  1 + (match->selectedPosition.x * unitSize) + offset[1],
                  (Color){ 255, 255, 255, 100 });
            }
          else
            {
              DrawTexture (
                  temp, 1 + (match->selectedPosition.y * unitSize) + offset[0],
                  1 + (match->selectedPosition.x * unitSize) + offset[1],
                  WHITE);
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

                  int posY, posX;

                  posY = 1 + (y * unitSize) + (u->animData.offY) + offset[0];
                  posX = 1 + (x * unitSize) + (u->animData.offX) + offset[1];

                  if (u->animationDb)
                    {

                      Texture2D *text = getUnitTexture (u);

                      if (!u->hasFormation)
                        {
                          DrawTexture (*text, posY, posX, brightness);
                        }
                      else
                        {
                          DrawTexture (*text, posY, posX,
                                       (Color){ 55, 55, 55, 255 });
                        }
                    }
                  else
                    {
                      DrawTexture (temp, posY, posX, WHITE);
                    }
                }
            }
        }

      DrawTexture (trees, 0, 0, WHITE);

      EndMode2D ();
      EndTextureMode ();

      BeginDrawing ();
      // ClearBackground (RED);

      BeginMode2D (screenSpaceCamera);
      DrawTexturePro (target.texture, sourceRec, destRec, origin, 0.0f, WHITE);

      EndMode2D ();

      if (match->debug)
        {
          DrawRectangle (5, 5, 340, 150, (Color){ 255, 255, 255, 200 });

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

          DrawText (TextFormat (
                        "movingDirection: [ %d, %d, %d, %d]",
                        match->movingDirection[0], match->movingDirection[1],
                        match->movingDirection[2], match->movingDirection[3]),
                    10, 90, 20, BLACK);

          DrawText (TextFormat (
                        "cooldownCounter: [ %d, %d, %d, %d]",
                        match->cooldownCounter[0], match->cooldownCounter[1],
                        match->cooldownCounter[2], match->cooldownCounter[3]),
                    10, 110, 20, BLACK);

          DrawText (TextFormat ("pauseCounter: [ %d, %d, %d, %d]",
                                match->pauseCounter[0], match->pauseCounter[1],
                                match->pauseCounter[2],
                                match->pauseCounter[3]),
                    10, 130, 20, BLACK);
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
