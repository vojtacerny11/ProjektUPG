#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <vector>

#define SCREENWIDTH 1900
#define SCREENHEIGHT 1000

#define CellHeight 250
#define CellWidth 250

unsigned short AnimationDelay = 5;
unsigned short AnimationDelayCount = 0;
unsigned short NotMoving = 40;
unsigned short NotMovingCount = 0;
unsigned short Health = 0;
unsigned short fallVelocity = 15;
int playerIndex = 0;
bool IsFacingRight = 1;
bool IsFalling = 0;


Vector2 playerPos = { SCREENWIDTH / 5, SCREENHEIGHT - CellHeight};
Vector2 opponentPos = { SCREENWIDTH, SCREENHEIGHT - CellHeight };

Texture2D spritePlayer;
Texture2D spriteOpponent;
Texture2D floorSprite;
Texture2D hearts;
Texture2D heartHalf;
Texture2D heartPrazdne;

Rectangle source = (Rectangle{ 0, 0, 250, 250 });
Rectangle sourceL = (Rectangle{ 0, 250, 250, 250 });
Rectangle sourceH = (Rectangle{ 0, 0, 150, 70 });
Rectangle sourceHh = (Rectangle{ 0, 0, 50, 70 });
Rectangle FloorRec = (Rectangle{ 0, 0, 2000, 50 });

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
	Sound death = LoadSound("audio/death.wav");
	SetSoundVolume(step, 0.25f);
	SetSoundVolume(death, 0.5f);
	SetMusicVolume(music, 0.5f);
	PlayMusicStream(music);
	SetTargetFPS(60);
	spritePlayer = LoadTexture("assets/person2.png");
	hearts = LoadTexture("assets/hearts.png");
	heartHalf = LoadTexture("assets/heart_half.png");
	heartPrazdne = LoadTexture("assets/heart_prazdne.png");
	floorSprite = LoadTexture("assets/floor1.png");
	while (WindowShouldClose() == 0)
	{
		UpdateMusicStream(music);
		++AnimationDelayCount;
		NotMovingCount++;
		if (NotMovingCount > NotMoving)
		{
			if (IsFacingRight)
				source.x = 0, 0;
			else
				sourceL.x = 700, 700;
		}
		if (NotMovingCount < NotMoving)
		{
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
		}
		BeginDrawing();
		if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
		{
			PlaySound(step);
			playerPos.x += CellWidth;
			IsFalling = 1;
			NotMovingCount = 0;
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
			IsFalling = 1;
			NotMovingCount = 0;
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
			IsFalling = 1;
			NotMovingCount = 0;
		}
		if (!IsFalling)
		{
			if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
			{
				PlaySound(step);
				playerPos.y -= CellHeight;
				IsFalling = 1;
				NotMovingCount = 0;
				if (playerPos.y < 0)
				{
					playerPos.y += CellHeight;
				}
			}
		}
		if (IsFalling)
		{
			playerPos.y += fallVelocity;
			if (playerPos.y > SCREENHEIGHT - CellHeight) {
				IsFalling = 0;
				playerPos.y = SCREENHEIGHT - CellHeight;
			}
		}
		ClearBackground(RAYWHITE);
		DrawTextureRec(floorSprite, FloorRec, (Vector2{ 0, SCREENHEIGHT - 50 }), WHITE);
		if (IsFacingRight)
		DrawTextureRec(spritePlayer, source, playerPos, WHITE);
		if (!IsFacingRight)
		DrawTextureRec(spritePlayer, sourceL, playerPos, WHITE);
		DrawTextureRec(hearts, sourceH, (Vector2{ 20, 0 }), WHITE);
		if (Health == 83) {
			sourceH = (Rectangle{ 0, 0, 100, 70 });
			DrawTextureRec(heartHalf, sourceHh, (Vector2{ 120, 3 }), WHITE);
		}
		else if (Health == 66)
		{
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 120, 2 }), WHITE);
		}
		else if (Health == 49)
		{
			sourceH = (Rectangle{ 0, 0, 50, 70 });
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 120, 2 }), WHITE);
			DrawTextureRec(heartHalf, sourceHh, (Vector2{ 70, 3 }), WHITE);
		}
		else if (Health == 32) {
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 70, 2 }), WHITE);
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 120, 2 }), WHITE);
		}
		if (Health == 15) {
			DrawTextureRec(heartHalf, sourceHh, (Vector2{ 20, 3 }), WHITE);
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 70, 2 }), WHITE);
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 120, 2 }), WHITE);
		}
		if (Health < 15)
			PlaySound(death);
			break;
		if (playerPos.x == opponentPos.x && playerPos.y == opponentPos.y)
			Health -= 17;
		EndDrawing();
	}
	GAMEOVER();

	CloseAudioDevice();

	CloseWindow();

	return 0;
}