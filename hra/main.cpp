#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <vector>

#define SCREENWIDTH 1600
#define SCREENHEIGHT 900

#define CellHeight 100
#define CellWidth 100

unsigned short AnimationDelay = 5;
unsigned short AnimationDelayCount = 0;
unsigned short NotMoving = 30;
unsigned short NotMovingCount = 0;
unsigned short ShotDuration = 30;
unsigned short ShotDurationCount = 0;
unsigned short Health = 100;
unsigned short fallVelocity = 15;
short playerIndex = 0;
short shotIndex = 0;
bool IsFacingRight = 1;
bool IsFalling = 0;
bool IsShooting = 0;

Vector2 playerPos = { 100, SCREENHEIGHT - CellHeight - 50};
Vector2 opponentPos = { SCREENWIDTH, SCREENHEIGHT - CellHeight };

Texture2D spritePlayerR;
Texture2D spritePlayerL;
Texture2D spriteOpponent;
Texture2D floorSprite;
Texture2D hearts;
Texture2D heartHalf;
Texture2D heartPrazdne;
Texture2D spriteJumpR;
Texture2D spriteJumpL;
Texture2D spriteShoot;
Texture2D spriteShoot2;

Rectangle source = (Rectangle{ 0, 0, CellHeight, CellWidth });
Rectangle sourceL = (Rectangle{ 0, 0, CellHeight, CellWidth });
Rectangle sourceH = (Rectangle{ 0, 0, 150, 70 });
Rectangle sourceHh = (Rectangle{ 0, 0, 50, 70 });
Rectangle FloorRec = (Rectangle{ 0, 0, 2000, 50 });
Rectangle sourceJump = (Rectangle{ 0, 0, CellWidth, 150 });

Sound step;
Sound gunshot;
Sound hit;

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

void UpdatePlayer();
void UpdateHealth();

int main()
{
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "2DWESTERN");
	InitAudioDevice();
	Music music = LoadMusicStream("audio/everet-almond.mp3");
	//all music rights reserved to everet almond
	Sound death = LoadSound("audio/death.wav");
	hit = LoadSound("audio/hit.wav");
	step = LoadSound("audio/step.wav");
	gunshot = LoadSound("audio/gunshot.mp3");
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
	spriteJumpR = LoadTexture("assets/spriteJump.png");
	spriteJumpL = LoadTexture("assets/spriteJump2.png");
	spriteShoot = LoadTexture("assets/spriteShooting.png");
	spriteShoot2 = LoadTexture("assets/spriteShooting2.png");
	while (WindowShouldClose() == 0)
	{
		UpdateMusicStream(music);
		++ShotDurationCount;
		++AnimationDelayCount;
		NotMovingCount++;

		BeginDrawing();

		UpdatePlayer();
		
		if (IsKeyPressed(KEY_M))
			SetMusicVolume(music, 0.0f);

		if (IsKeyPressed(KEY_U))
			SetMusicVolume(music, 0.5f);

		ClearBackground(WHITE);
		DrawTextureRec(floorSprite, FloorRec, (Vector2{ 0, SCREENHEIGHT - 50 }), WHITE);
		if (IsShooting)
		{
			if (IsFacingRight) {
				if (ShotDuration > ShotDurationCount)
					DrawTextureRec(spriteShoot, source, playerPos, WHITE);
				else
					IsShooting = 0;
			}
			if (!IsFacingRight) {
				if (ShotDuration > ShotDurationCount)
					DrawTextureRec(spriteShoot2, source, playerPos, WHITE);
				else
					IsShooting = 0;
			}
		}
		else if (IsFacingRight && IsFalling)
			DrawTextureRec(spriteJumpR, sourceJump, playerPos, WHITE);
		else if (!IsFacingRight && IsFalling)
			DrawTextureRec(spriteJumpL, sourceJump, playerPos, WHITE);
		else if (IsFacingRight && IsFalling == 0)
			DrawTextureRec(spritePlayerR, source, playerPos, WHITE);
		else if (!IsFacingRight && IsFalling == 0)
			DrawTextureRec(spritePlayerL, sourceL, playerPos, WHITE);

		UpdateHealth();

		DrawTextureRec(hearts, sourceH, (Vector2{ 20, 0 }), WHITE);
		if (Health < 15) {
			PlaySound(death);
			break;
		}
		EndDrawing();
	}
	GAMEOVER();

	CloseAudioDevice();

	CloseWindow();

	return 0;
}

void UpdatePlayer()
{
	if (NotMovingCount > NotMoving)
	{
		if (IsFacingRight)
			source.x = 0, 0;
		else
			sourceL.x = 200, 0;
	}
	if (NotMovingCount < NotMoving && IsFalling == 0)
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
	switch (IsFalling)
	{
	case 0:
		if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
		{
			playerPos.y -= 3 * CellHeight;
			IsFalling = 1;
		}
		if (IsKeyPressed(KEY_SPACE))
		{
			ShotDurationCount = 0;
			IsShooting = 1;
			PlaySound(gunshot);
		}
		break;
	case 1:
		playerPos.y += fallVelocity;
		if (playerPos.y > SCREENHEIGHT - CellHeight - 50 - 30) { //nohy se nepropadaji
			IsFalling = 0;
			PlaySound(step);
			playerPos.y = SCREENHEIGHT - CellHeight - 50;
			break;
		}
	}
}

void UpdateHealth()
{
	switch (Health) {
		case 83:
			sourceH = (Rectangle{ 0, 0, 100, 70 });
			DrawTextureRec(heartHalf, sourceHh, (Vector2{ 120, 3 }), WHITE);
			break;
		case 66:
			sourceH = (Rectangle{ 0, 0, 100, 70 });
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 120, 2 }), WHITE);
			break;
		case 49:
			sourceH = (Rectangle{ 0, 0, 50, 70 });
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 120, 2 }), WHITE);
			DrawTextureRec(heartHalf, sourceHh, (Vector2{ 70, 3 }), WHITE);
			break;
		case 32:
			sourceH = (Rectangle{ 0, 0, 50, 70 });
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 70, 2 }), WHITE);
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 120, 2 }), WHITE);
			break;
		case 15:
			sourceH = (Rectangle{ 0, 0, 0, 70 });
			DrawTextureRec(heartHalf, sourceHh, (Vector2{ 20, 3 }), WHITE);
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 70, 2 }), WHITE);
			DrawTextureRec(heartPrazdne, sourceHh, (Vector2{ 120, 2 }), WHITE);
			break;
	}
	if (playerPos.x == opponentPos.x && playerPos.y == opponentPos.y) {
		Health -= 17;
		PlaySound(hit);
	}
}