#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <vector>

#define SCREENWIDTH 1900
#define SCREENHEIGHT 1000

#define CellHeight 100
#define CellWidth 100

unsigned short AnimationDelay = 5;
unsigned short AnimationDelayCount = 0;
unsigned short NotMoving = 40;
unsigned short NotMovingCount = 0;
unsigned short Health = 100;
unsigned short fallVelocity = 15;
int playerIndex = 0;
bool IsFacingRight = 1;
bool IsFalling = 0;


Vector2 playerPos = { 100, SCREENHEIGHT - CellHeight - 50};
Vector2 opponentPos = { SCREENWIDTH, SCREENHEIGHT - CellHeight };

Texture2D spritePlayerR;
Texture2D spritePlayerL;
Texture2D spriteOpponent;
Texture2D floorSprite;
Texture2D hearts;
Texture2D heartHalf;
Texture2D heartPrazdne;

Rectangle source = (Rectangle{ 0, 0, CellHeight, CellWidth });
Rectangle sourceL = (Rectangle{ 0, 0, CellHeight, CellWidth });
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
	Sound hit = LoadSound("audio/hit.wav");
	Sound gunshot = LoadSound("audio/gunshot.mp3");
	SetSoundVolume(step, 0.35f);
	SetSoundVolume(death, 0.5f);
	SetSoundVolume(hit, 0.5f);
	SetSoundVolume(gunshot, 0.5f);
	SetMusicVolume(music, 0.5f);
	PlayMusicStream(music);
	SetTargetFPS(60);
	spritePlayerR = LoadTexture("assets/sprite.png");
	spritePlayerL = LoadTexture("assets/sprite2.png");
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
				sourceL.x = 200, 0;
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
			if (!IsFalling)
				PlaySound(step);
			playerPos.x += CellWidth;
			IsFalling = 1;
			NotMovingCount = 0;
			if (playerPos.x > SCREENWIDTH - CellWidth)
			{
				playerPos.x = 0;
			}
			IsFacingRight = 1;
		}
		if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
		{
			if (!IsFalling)
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
		if (!IsFalling)
		{
			if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
			{
				playerPos.y -= 2 * CellHeight;
				IsFalling = 1;
				NotMovingCount = 0;
				if (playerPos.y < 0)
				{
					playerPos.y += CellHeight;
				}
			}
		}
		if (IsKeyPressed(KEY_SPACE))
		{
			PlaySound(gunshot);
		}
		if (IsFalling)
		{
			playerPos.y += fallVelocity;
			if (playerPos.y > SCREENHEIGHT - CellHeight - 50) {
				IsFalling = 0;
				PlaySound(step);
				playerPos.y = SCREENHEIGHT - CellHeight - 50;
			}
		}
		ClearBackground(RAYWHITE);
		DrawTextureRec(floorSprite, FloorRec, (Vector2{ 0, SCREENHEIGHT - 50 }), WHITE);
		if (IsFacingRight)
		DrawTextureRec(spritePlayerR, source, playerPos, WHITE);
		if (!IsFacingRight)
		DrawTextureRec(spritePlayerL, sourceL, playerPos, WHITE);
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
		if (Health < 15) {
			PlaySound(death);
			break;
		}
		if (playerPos.x == opponentPos.x && playerPos.y == opponentPos.y) {
			Health -= 17;
			PlaySound(hit);
		}
		EndDrawing();
	}
	GAMEOVER();

	CloseAudioDevice();

	CloseWindow();

	return 0;
}