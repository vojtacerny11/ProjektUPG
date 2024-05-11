#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
#include <vector>

#define SCREENWIDTH 1600
#define SCREENHEIGHT 900

#define CellHeight 100
#define CellWidth 100

Image icon = LoadImage("assets/cactus.png");

const unsigned short AnimationDelay = 5;
unsigned short AnimationDelayCount = 0;
const unsigned short NotMoving = 30;
unsigned short NotMovingCount = 0;
const unsigned short ShotDuration = 30;
unsigned short ShotDurationCount = 0;
short Health = 100;
const unsigned short fallVelocity = 15;

const unsigned short BushAnimationDelay = 6;
unsigned short BushAnimationDelayCount = 0;

const unsigned short BushNotOnScreen = 60;
unsigned short BushNotOnScreenCount = 0;

short playerIndex = 0;
short shotIndex = 0;
unsigned short rollIndex = 0;

unsigned short movementCount = 0;
const unsigned short movementDelay = 4; //movement neni moc rychly ani pomaly muzem si jeste pohrat 

unsigned short screenPassed = 0;

bool IsFacingRight = 1;
bool IsFalling = 0;
bool IsShooting = 0;
bool IsCactusFriendly = 0;
bool ShotPassed = 0;

const short worldHeight = 18;
const short worldWidth = 32;

int World[worldHeight][worldWidth] = {
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 1, 1,  1,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 1,  1,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 1},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 1,  1,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  1,  1, 0,  0,  0, 0, 0,  0,  0, 0, 0,  1,  1, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 1, 1,  0,  0, 1, 1,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 1,  1,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
	{ 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
};

Vector2 playerPos = { 100, SCREENHEIGHT - CellHeight - 50 };
Vector2 enemyPos = { SCREENWIDTH, SCREENHEIGHT - CellHeight - 50 };
Vector2 BushPos = { 0, SCREENHEIGHT - CellHeight };
Vector2 cactusPos = { SCREENWIDTH - CellHeight * 3, SCREENHEIGHT - CellHeight - 100 };
Vector2 bulletPos = { 200, SCREENHEIGHT - CellHeight - 50 };
Vector2 bullet2Pos = { 100, SCREENHEIGHT - CellHeight - 50 };

Texture2D pozadi;
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
Texture2D bush;
Texture2D enemyL;
Texture2D enemyR;
Texture2D cactus;
Texture2D salon;
Texture2D bullet;
Texture2D bullet2;
Texture2D plank;

Rectangle source = (Rectangle{ 0, 0, CellHeight, CellWidth });
Rectangle sourceL = (Rectangle{ 0, 0, CellHeight, CellWidth });
Rectangle sourceH = (Rectangle{ 0, 0, 150, 70 });
Rectangle sourceHh = (Rectangle{ 0, 0, 50, 70 });
Rectangle FloorRec = (Rectangle{ 0, 0, 2000, 50 });
Rectangle sourceJump = (Rectangle{ 0, 0, CellWidth, 150 });
Rectangle BushRec = (Rectangle{ 0, 0, CellWidth / 2, CellHeight / 2 });
Rectangle cactusRec = (Rectangle{ 0, 0, CellWidth, CellHeight + 50 });
Rectangle salonRec = (Rectangle{ 0, 0, 2 * CellWidth, 2 * CellHeight });
Rectangle bulletRec = (Rectangle{ 0, 0, CellWidth, CellHeight });
Rectangle plankRec = (Rectangle{ 0, 0, 50, 50 });


Sound step;
Sound gunshot;
Sound hit;
Sound MouseClick;

class ButtonPlay
{
public:
	void Draw()
	{
		DrawRectangle(SCREENWIDTH / 2 - 75, SCREENHEIGHT / 2 - 75, 150, 50, BLACK);
		DrawText("PLAY", SCREENWIDTH / 2 - 65, SCREENHEIGHT / 2 - 75, 50, WHITE);
	}
};

class ButtonQuit
{
public:
	void Draw()
	{
		DrawRectangle(SCREENWIDTH / 2 - 75, SCREENHEIGHT / 2 + 25, 150, 50, BLACK);
		DrawText("QUIT", SCREENWIDTH / 2 - 60, SCREENHEIGHT / 2 + 25, 50, WHITE);
	}
};

class Map1
{
public:
	void Draw()
	{
		DrawRectangle(SCREENWIDTH / 2 - 130, SCREENHEIGHT / 2 - 75, 300, 50, BLACK);
		DrawText("CactusRun", SCREENWIDTH / 2 - 115, SCREENHEIGHT / 2 - 75, 50, WHITE);
	}
};

class Map2
{
public:
	void Draw()
	{
		DrawRectangle(SCREENWIDTH / 2 - 130, SCREENHEIGHT / 2 + 25, 300, 50, BLACK);
		DrawText("NightJump", SCREENWIDTH / 2 - 100, SCREENHEIGHT / 2 + 25, 50, WHITE);
	}
};

void MainMenu();

void ChooseMap();

void GamePlay();

void GAMEOVER()
{
	Sound womp = LoadSound("audio/womp.mp3");
	bool wasplayed = 0;
	while (WindowShouldClose() == 0)
	{
		if (!wasplayed) {
			PlaySound(womp);
			wasplayed = 1;
		}
		BeginDrawing();
		DrawText("YOU LOST", SCREENWIDTH / 2 - 250 / 2, SCREENHEIGHT / 2 - 50, 50, BLACK);
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
}

void FINISH()
{
	Sound win = LoadSound("audio/win.mp3");
	bool wasplayed = 0;
	while (WindowShouldClose() == 0)
	{
		if (!wasplayed) {
			PlaySound(win);
			wasplayed = 1;
		}
		BeginDrawing();
		DrawText("YOU WON", SCREENWIDTH / 2 - 225 / 2, SCREENHEIGHT / 2 - 50, 50, BLACK);
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
}

void UpdatePlayer();
void UpdateHealth();

int GetTile();

void CheckPlatform();

void drawPlatform();

void resetWorld();

int main()
{
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "2DWESTERN");
	SetWindowIcon(icon);
	InitAudioDevice();
	MainMenu();

	return 0;
}

void MainMenu()
{
	SetTargetFPS(60);
	ButtonPlay play = ButtonPlay();
	ButtonQuit quit = ButtonQuit();
	MouseClick = LoadSound("audio/click.mp3");
	bool ShouldQuit = 0;
	while (!ShouldQuit && !WindowShouldClose())
	{
		BeginDrawing();
		play.Draw();
		quit.Draw();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			int x = GetMouseX();
			int y = GetMouseY();
			if ((x > SCREENWIDTH / 2 - 75 && y > SCREENHEIGHT / 2 - 75) && (x < SCREENWIDTH / 2 + 75 && y < SCREENHEIGHT / 2 - 25)) {
				PlaySound(MouseClick);
				ChooseMap();
				ShouldQuit = 1;
			}
			if ((x > SCREENWIDTH / 2 - 75 && y > SCREENHEIGHT / 2 + 25) && (x < SCREENWIDTH / 2 + 75 && y < SCREENHEIGHT / 2 + 75))
				ShouldQuit = 1;
		}
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
}

void ChooseMap()
{
	Map1 CactusRun = Map1();
	Map2 NightJump = Map2();
	unsigned short sleepedFor = 0;
	unsigned short sleep = 5;
	bool ShouldQuit = 0;
	while (!ShouldQuit && !WindowShouldClose())
	{
		sleepedFor++;
		BeginDrawing();
		CactusRun.Draw();
		NightJump.Draw();
		if (sleepedFor > sleep && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			int x = GetMouseX();
			int y = GetMouseY();
			if ((x > SCREENWIDTH / 2 - 130 && y > SCREENHEIGHT / 2 - 75) && (x < SCREENWIDTH / 2 + 170 && y < SCREENHEIGHT / 2 - 25)) {
				PlaySound(MouseClick);
				GamePlay();
				ShouldQuit = 1;
			}
			if ((x > SCREENWIDTH / 2 - 130 && y > SCREENHEIGHT / 2 + 25) && (x < SCREENWIDTH / 2 + 170 && y < SCREENHEIGHT / 2 + 75)) {
				PlaySound(MouseClick);
				DrawText("ahoj", 0, 0, 50, BLACK);
				//ShouldQuit = 1;
			}
		}
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
	CloseWindow();
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
	if (movementCount > movementDelay) {
		movementCount = 0;
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		{
			if (!IsFalling)
				PlaySound(step);
			playerPos.x += CellWidth;
			bulletPos.x += CellWidth;
			bullet2Pos.x += CellWidth;
			IsFalling = 1;
			NotMovingCount = 0;
			if (playerPos.x > SCREENWIDTH - CellWidth)
			{
				screenPassed++;
				resetWorld();
				IsCactusFriendly = 0;
				playerPos.x = 0;
				int random = rand() % 3 + 2;
				cactusPos.x -= CellWidth * random;
				if (cactusPos.x < 0) {
					cactusPos.x = SCREENWIDTH - CellHeight * 3;
				}
			}
			IsFacingRight = 1;
		}
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		{
			if (!IsFalling)
				PlaySound(step);
			playerPos.x -= CellWidth;
			bulletPos.x -= CellWidth;
			bullet2Pos.x -= CellWidth;
			IsFalling = 1;
			NotMovingCount = 0;
			if (playerPos.x < 0)
			{
				playerPos.x += CellWidth;
			}
			IsFacingRight = 0;
		}
	}
	switch (IsFalling)
	{
	case 0:
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		{
			playerPos.y -= 3 * CellHeight;
			IsFalling = 1;
		}
		if (IsKeyPressed(KEY_SPACE))
		{
			ShotDurationCount = 0;
			IsShooting = 1;
			ShotPassed = 0;
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
	if (playerPos.x == enemyPos.x && playerPos.y == enemyPos.y) {
		Health -= 17;
		PlaySound(hit);
	}
	if (playerPos.x == cactusPos.x && playerPos.y >= SCREENHEIGHT - CellHeight * 3 && !IsCactusFriendly) { //podminka kvuli fallvelocity
		Health -= 17;
		PlaySound(hit);
		IsCactusFriendly = 1;
	}
}

void GamePlay()
{
	Music music = LoadMusicStream("audio/everet-almond.mp3");
	//all music rights reserved to everet almond
	Sound death = LoadSound("audio/death.wav");
	hit = LoadSound("audio/hit.mp3");
	step = LoadSound("audio/step.wav");
	gunshot = LoadSound("audio/gunshot.mp3");
	SetSoundVolume(step, 0.35f);
	SetSoundVolume(death, 0.5f);
	SetSoundVolume(hit, 0.5f);
	SetSoundVolume(gunshot, 0.5f);
	SetMusicVolume(music, 0.5f);
	PlayMusicStream(music);
	SetTargetFPS(60);
	pozadi = LoadTexture("assets/pozadi.png");
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
	bush = LoadTexture("assets/RollingBush2.png");
	enemyR = LoadTexture("assets/sprite_enemy.png");
	enemyL = LoadTexture("assets/sprite_enemy2.png");
	cactus = LoadTexture("assets/cactuslowquality.png");
	salon = LoadTexture("assets/salon.png");
	bullet = LoadTexture("assets/bullet.png");
	bullet2 = LoadTexture("assets/bullet2.png");
	plank = LoadTexture("assets/plank.png");
	while (WindowShouldClose() == 0)
	{
		UpdateMusicStream(music);
		++ShotDurationCount;
		++AnimationDelayCount;
		++BushAnimationDelayCount;
		NotMovingCount++;
		movementCount++;

		BeginDrawing();

		if (BushAnimationDelayCount > BushAnimationDelay)
		{
			BushAnimationDelayCount = 0;
			++rollIndex;
			rollIndex %= 5;
			BushRec.x = (float)CellWidth / 2 * (float)rollIndex;
			BushPos.x += CellWidth / 2;
			if (BushPos.x > SCREENWIDTH - CellWidth)
			{
				++BushNotOnScreenCount;
				if (BushNotOnScreenCount > BushNotOnScreen) {
					BushNotOnScreenCount = 0;
					BushPos.x = 0;
				}
			}
		}

		UpdatePlayer();

		if (IsKeyPressed(KEY_M))
			SetMusicVolume(music, 0.0f);

		if (IsKeyPressed(KEY_U))
			SetMusicVolume(music, 0.5f);
		DrawTexture(pozadi, 0, 0, WHITE);

		DrawTextureRec(floorSprite, FloorRec, (Vector2{ 0, SCREENHEIGHT - 50 }), WHITE);

		DrawTextureRec(cactus, cactusRec, cactusPos, WHITE);

		if (screenPassed == 9) {
			DrawTextureRec(salon, salonRec, (Vector2{ SCREENWIDTH - CellWidth * 2, SCREENHEIGHT - CellHeight * 2.5 }), WHITE);
			if (playerPos.x >= SCREENWIDTH - CellWidth && !IsFalling)
				screenPassed++;
		}
		if (IsShooting)
		{
			if (IsFacingRight) {
				if (ShotDuration > ShotDurationCount)
				{
					DrawTextureRec(spriteShoot, source, playerPos, WHITE);
					if (!ShotPassed) {
						DrawTextureRec(bullet, bulletRec, bulletPos, WHITE);
						bulletPos.x += CellWidth;
					}
					if (bulletPos.x > SCREENWIDTH - CellWidth)
					{
						bulletPos.x = playerPos.x + CellWidth;
						ShotPassed = 1;
					}
				}
				else
					IsShooting = 0;
			}
			if (!IsFacingRight) {
				if (ShotDuration > ShotDurationCount)
				{
					DrawTextureRec(spriteShoot2, source, playerPos, WHITE);
					if (!ShotPassed) {
						DrawTextureRec(bullet2, bulletRec, bullet2Pos, WHITE);
						bullet2Pos.x -= CellWidth;
					}
					if (bullet2Pos.x < 0)
					{
						bullet2Pos.x = playerPos.x - CellWidth;
						ShotPassed = 1;
					}
				}
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

		DrawTextureRec(bush, BushRec, BushPos, WHITE);

		UpdateHealth();

		DrawTextureRec(hearts, sourceH, (Vector2{ 20, 0 }), WHITE);

		if (screenPassed == 10)
			break;
		if (Health < 15) {
			PlaySound(death);
			break;
		}
		EndDrawing();
	}
	UnloadTexture(pozadi);
	UnloadTexture(spritePlayerR);
	UnloadTexture(spritePlayerL);
	UnloadTexture(spriteOpponent);
	UnloadTexture(floorSprite);
	UnloadTexture(hearts);
	UnloadTexture(heartHalf);
	UnloadTexture(heartPrazdne);
	UnloadTexture(spriteJumpR);
	UnloadTexture(spriteJumpL);
	UnloadTexture(spriteShoot);
	UnloadTexture(spriteShoot2);
	UnloadTexture(bush);
	UnloadTexture(enemyL);
	UnloadTexture(enemyR);
	UnloadTexture(cactus);
	UnloadTexture(salon);
	UnloadTexture(bullet);
	UnloadTexture(bullet2);
	UnloadTexture(plank);
	if (screenPassed < 10)
		GAMEOVER();
	FINISH();
	CloseAudioDevice();

	CloseWindow();
}

void drawPlatform()
{
	for (int row = 0; row < worldHeight; row++)
		for (int col = 0; col < worldWidth; col++)
		{
			if (World[row][col] == 1)
			{
				DrawTextureRec(plank, plankRec, (Vector2{ (float)col * CellWidth / 2, (float)row * CellHeight / 2 }), WHITE);
			}
		}
}

int GetTile(unsigned row, unsigned col)
{
	return World[col][row];
}

void CheckPlatform()
{
	if (IsFacingRight) {
		if (GetTile(playerPos.x / 50, playerPos.y / 50 + 2 - (30 / 50)) == 1)
			IsFalling = 0;
	}
	if (!IsFacingRight) {
		if (GetTile(playerPos.x / 50 + 1, playerPos.y / 50 + 2 - (30 / 50)) == 1)
			IsFalling = 0;
	}
}

void resetWorld()
{
	for (int i = 0; i < worldHeight; i++) {
		if (World[i][worldWidth - 1] == 1) {
			World[i][0] = World[i][worldWidth - 1];
			World[i][1] = World[i][worldWidth - 1];
		}
	}
	for (int i = 2; i < worldWidth; i++) {
		for (int j = 0; j < worldHeight; j++) {
			World[j][i] = 0;
		}
	}
}