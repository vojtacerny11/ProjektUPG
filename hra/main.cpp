#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <vector>

#define SCREENWIDTH 1250
#define SCREENHEIGHT 750

#define CellHeight 250
#define CellWidth 245

unsigned AnimationDelay = 5;
unsigned AnimationDelayCount = 0;
int playerIndex = 0;
bool IsFacingRight = 1;

Vector2 playerPos = { SCREENWIDTH / 5, SCREENHEIGHT - CellHeight};

Texture2D spritePlayer;
Texture2D spriteOpponent;

Rectangle source = (Rectangle{ 0, 0, 250, 250 });
Rectangle sourceL = (Rectangle{ 0, 250, 250, 250 });
int main()
{
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "2DSHOOTER");
	InitAudioDevice();
	Music music = LoadMusicStream("audio/Night.mp3");
	PlayMusicStream(music);
	SetTargetFPS(60);
	spritePlayer = LoadTexture("assets/person2.png");
	while (WindowShouldClose() == 0)
	{
		UpdateMusicStream(music);
		++AnimationDelayCount;
			if (AnimationDelayCount > AnimationDelay)
			{
				AnimationDelayCount = 0;
				++playerIndex;
				if (playerIndex == 3)
					playerIndex = 0;
				if (IsFacingRight)
				source.x = (float)CellWidth * (float)playerIndex;
				if (!IsFacingRight)
				sourceL.x = (float)CellWidth * (float)playerIndex;
			}
		BeginDrawing();
		if (IsKeyPressed(KEY_D))
		{
			playerPos.x += CellWidth;
			//if (playerPos.x < 0 || playerPos.y < 0 || playerPos.x > SCREENWIDTH || playerPos.y > SCREENHEIGHT - CellHeight)
			if (playerPos.x > SCREENWIDTH - CellWidth)
			{
				playerPos.x -= CellWidth;
			}
			IsFacingRight = 1;
		}
		if (IsKeyPressed(KEY_A))
		{
			playerPos.x -= CellWidth;
			if (playerPos.x < 0)
			{
				playerPos.x += CellWidth;
			}
			IsFacingRight = 0;
		}
		if (IsKeyPressed(KEY_S))
		{
			playerPos.y += CellHeight;
			if (playerPos.y > SCREENHEIGHT - CellHeight)
			{
				playerPos.y -= CellHeight;
			}
		}
		if (IsKeyPressed(KEY_W))
		{
			playerPos.y -= CellHeight;
			if (playerPos.y < 0)
			{
				playerPos.y += CellHeight;
			}
		}
		ClearBackground(RAYWHITE);
		if (IsFacingRight)
		DrawTextureRec(spritePlayer, source, playerPos, WHITE);
		if (!IsFacingRight)
		DrawTextureRec(spritePlayer, sourceL, playerPos, WHITE);
		EndDrawing();
	}


	CloseAudioDevice();

	CloseWindow();

	return 0;
}