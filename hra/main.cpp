#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <vector>

#define SCREENWIDTH 1250
#define SCREENHEIGHT 750

#define CellHeight 250
#define CellWidth 250

unsigned AnimationDelay = 5;
unsigned AnimationDelayCount = 0;
unsigned Health = 50;
int playerIndex = 0;
bool IsFacingRight = 1;

Vector2 playerPos = { SCREENWIDTH / 5, SCREENHEIGHT - CellHeight};
Vector2 opponentPos = { SCREENWIDTH, SCREENHEIGHT - CellHeight };

Texture2D spritePlayer;
Texture2D spriteOpponent;
Texture2D healthBar;
Texture2D healthBarHalf;

Rectangle source = (Rectangle{ 0, 0, 250, 250 });
Rectangle sourceL = (Rectangle{ 0, 250, 250, 250 });
Rectangle sourceH = (Rectangle{ 0, 0, 300, 150 });

void GAMEOVER()
{
	Music music2 = LoadMusicStream("audio/titanic.mp3");
	PlayMusicStream(music2);
	while (WindowShouldClose() == 0)
	{
		UpdateMusicStream(music2);
		BeginDrawing();
		DrawText("YOU LOST", SCREENWIDTH / 2 - 225 / 2, SCREENHEIGHT / 2 - 50, 50, BLACK);
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
}

int main()
{
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "2DSHOOTER");
	InitAudioDevice();
	Music music = LoadMusicStream("audio/Night.mp3");
	Sound step = LoadSound("audio/step.wav");
	SetSoundVolume(step, 0.5f);
	SetMusicVolume(music, 0.75f);
	PlayMusicStream(music);
	SetTargetFPS(60);
	spritePlayer = LoadTexture("assets/person2.png");
	healthBar = LoadTexture("assets/healthbarfull.png");
	healthBarHalf = LoadTexture("assets/healthbarhalf.png");
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
		if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
		{
			PlaySound(step);
			playerPos.x += CellWidth;
			//if (playerPos.x < 0 || playerPos.y < 0 || playerPos.x > SCREENWIDTH || playerPos.y > SCREENHEIGHT - CellHeight)
			if (playerPos.x > SCREENWIDTH - CellWidth)
			{
				playerPos.x -= CellWidth;
			}
			IsFacingRight = 1;
		}
		if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
		{
			PlaySound(step);
			playerPos.x -= CellWidth;
			if (playerPos.x < 0)
			{
				playerPos.x += CellWidth;
			}
			IsFacingRight = 0;
		}
		if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
		{
			PlaySound(step);
			playerPos.y += CellHeight;
			if (playerPos.y > SCREENHEIGHT - CellHeight)
			{
				playerPos.y -= CellHeight;
			}
		}
		if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
		{
			PlaySound(step);
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
		if (Health == 100)
		DrawTextureRec(healthBar, sourceH, (Vector2{ 0, 0 }), WHITE);
		if (Health == 50)
			DrawTextureRec(healthBarHalf, sourceH, (Vector2{ 0, 0 }), WHITE);
		if (playerPos.x == opponentPos.x && playerPos.y == opponentPos.y)
			break;
			//Health -= 50;
		const char* cHealth = TextFormat("%d", Health);
		DrawText(cHealth, 115, 50, 50, BLACK);
		EndDrawing();
	}
	GAMEOVER();

	CloseAudioDevice();

	CloseWindow();

	return 0;
}