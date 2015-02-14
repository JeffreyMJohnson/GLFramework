#include "Globals.h"
#include <string>

#include <time.h>
#include <vector>

#include "Framework.h"
#include "Player.h"
//#include "Platform.h"

#define Abs(x) ((x) < 0 ? -(x) : (x))
#define Max(a, b) ((a) > (b) ? (a) : (b))

//struct vec2
//{
//	float x = 0.0f;
//	float y = 0.0f;
//};

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const float TOLERANCE = .0001f;

GLFWwindow* window;
double totalTime;
double deltaTime;
bool quit = false;

float gravity = 35;


float playerMoveSpeed = 250;
float playerJumpVelocity = 500;

Player player;
//uint ground;

std::vector<Platform> platformList;
typedef std::vector<Platform>::iterator PlatIt;

void Initialize();
void Destroy();
void HandleUI();
void ResetDeltaTime();
bool IsPlayerCollided();
bool RelDiff(float lhs, float rhs);
void LoadPlatforms();
void DrawPlatforms();
void PlayerLogic();
void CheckPlatformCollision();

Font font;

Framework frk;

typedef unsigned int uint;

int main()
{


	frk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Framework Test");
	frk.SetBackgroundColor(.482, .698, 1, 1);

	LoadPlatforms();

	player.Init(vec2(100, 100), vec2(100, 200));
	player.mColliderOffset = vec2(.1, .45);
	player.mSpriteID = frk.CreateAnimation(player.mSize.x, player.mSize.y, ".\\resources\\images\\smurf_sprite.xml");
	frk.MoveAnimation(player.mSpriteID, player.mPosition.x, player.mPosition.y);
	player.UpdateCollider();


	do{
		deltaTime = frk.GetDeltaTime();
		frk.ClearScreen();

		

		player.Update(deltaTime, platformList);
		frk.SetAnimationState(player.mSpriteID, player.mCurrentAnimationState);
		HandleUI();
		frk.MoveAnimation(player.mSpriteID, player.mPosition.x += player.mVelocity.x * deltaTime,
			player.mPosition.y += player.mVelocity.y * deltaTime);


		frk.DrawText("I am a User Interface", SCREEN_WIDTH * .5, SCREEN_HEIGHT * .95);
		DrawPlatforms();
		frk.DrawAnimation(player.mSpriteID);

		

	} while (frk.UpdateFramework() && !quit);

	frk.Shutdown();

	return 0;
}

bool RelDiff(float lhs, float rhs)
{
	float c = Abs(lhs);
	float d = Abs(rhs);

	d = Max(c, d);
	return d == 0.0 ? 0.0 : Abs((lhs - rhs)) / d;
}

void LoadPlatforms()
{
	//uint ground = frk.CreateSprite(1200, 32, ".\\resources\\images\\platform_long.png", true);
	//frk.MoveSprite(ground, 512, 15);

	Platform groundLeft;
	groundLeft.Init(vec2(400, 32), vec2(100, 15));
	groundLeft.mSpriteID = frk.CreateSprite(groundLeft.mSize.x, groundLeft.mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(groundLeft.mSpriteID, groundLeft.mPosition.x, groundLeft.mPosition.y);
	platformList.push_back(groundLeft);

	Platform groundRight;
	groundRight.Init(vec2(400, 32), vec2(850, 15));
	groundRight.mSpriteID = frk.CreateSprite(groundRight.mSize.x, groundRight.mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(groundRight.mSpriteID, groundRight.mPosition.x, groundRight.mPosition.y);
	platformList.push_back(groundRight);

	Platform groundCenter;
	groundCenter.Init(vec2(200, 32), vec2(500, 15));
	groundCenter.mSpriteID = frk.CreateSprite(groundCenter.mSize.x, groundCenter.mSize.y, ".\\resources\\images\\platform_short.png", true);
	frk.MoveSprite(groundCenter.mSpriteID, groundCenter.mPosition.x, groundCenter.mPosition.y);
	platformList.push_back(groundCenter);

}

void HandleUI()
{

	if (frk.IsKeyDown(SPACE) && !player.mJumping)
	{
		player.mJumping = true;
 		player.mVelocity.y = 1000;
	}

	if (frk.IsKeyDown(ESC))
	{
		quit = true;
	}

	if (frk.IsKeyDown(A))
	{
		if (player.mDirection == 1)
		{
			player.mDirection = -1;
			frk.AnimationFlipDirection(player.mSpriteID);

		}
		frk.SetAnimationState(player.mSpriteID, "walk");
		//playerPosition.x -= playerMoveSpeed * frk.GetDeltaTime();
		player.mVelocity.x = playerMoveSpeed * player.mDirection;
	}

	if (frk.IsKeyDown(D))
	{
		if (player.mDirection == -1)
		{
			player.mDirection = 1;
			frk.AnimationFlipDirection(player.mSpriteID);
		}
		frk.SetAnimationState(player.mSpriteID, "walk");
		player.mVelocity.x = playerMoveSpeed * player.mDirection;

	}

}

void DrawPlatforms()
{
	for (PlatIt it = platformList.begin(); it != platformList.end(); it++)
	{
		frk.DrawSprite(it->mSpriteID);
	}
}