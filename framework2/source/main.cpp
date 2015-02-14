#include "Globals.h"
#include <string>

#include <time.h>
#include <vector>

#include "Framework.h"
#include "Player.h"
#include "MovingPlatform.h"

#define Abs(x) ((x) < 0 ? -(x) : (x))
#define Max(a, b) ((a) > (b) ? (a) : (b))

//struct vec2
//{
//	float x = 0.0f;
//	float y = 0.0f;
//};

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const float DEBUG_TIME_DELTA = 1.0f / 100;

GLFWwindow* window;
double totalTime;
double deltaTime;
bool quit = false;
bool jumpKeyPressed = false;

float gravity = 35;


float playerMoveSpeed = 250;
float playerJumpVelocity = 500;

Player player;
//uint ground;

std::vector<Platform*> platformList;
typedef std::vector<Platform*>::iterator PlatIt;

void Initialize();
void Destroy();
void HandleUI();
void ResetDeltaTime();
bool IsPlayerCollided();
bool RelDiff(float lhs, float rhs);
void LoadPlatforms();
void UpdatePlatforms(float timeDelta);
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
	player.mColliderOffset = vec2(.15, .45);
	player.mSpriteID = frk.CreateAnimation(player.mSize.x, player.mSize.y, ".\\resources\\images\\smurf_sprite.xml");
	frk.MoveAnimation(player.mSpriteID, player.mPosition.x, player.mPosition.y);
	player.UpdateCollider();



	do{
		//debug
		//deltaTime = frk.GetDeltaTime();
		deltaTime = DEBUG_TIME_DELTA;
		frk.ClearScreen();

		

		player.Update(deltaTime, platformList);
		frk.SetAnimationState(player.mSpriteID, player.mCurrentAnimationState);
		HandleUI();
		frk.MoveAnimation(player.mSpriteID, player.mPosition.x += player.mVelocity.x * deltaTime,
			player.mPosition.y += player.mVelocity.y * deltaTime);


		frk.DrawText("I am a User Interface", SCREEN_WIDTH * .5, SCREEN_HEIGHT * .95);
		
		UpdatePlatforms(deltaTime);
		DrawPlatforms();
		frk.DrawAnimation(player.mSpriteID);
		player.UpdateCollider();
		

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

	Platform* groundLeft = new Platform;
	groundLeft->Init(vec2(400, 32), vec2(100, 15));
	groundLeft->mSpriteID = frk.CreateSprite(groundLeft->mSize.x, groundLeft->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(groundLeft->mSpriteID, groundLeft->mPosition.x, groundLeft->mPosition.y);
	platformList.push_back(groundLeft);



	Platform* groundCenter = new Platform;
	groundCenter->Init(vec2(175, 32), vec2(475, 15));
	groundCenter->mSpriteID = frk.CreateSprite(groundCenter->mSize.x, groundCenter->mSize.y, ".\\resources\\images\\platform_short.png", true);
	frk.MoveSprite(groundCenter->mSpriteID, groundCenter->mPosition.x, groundCenter->mPosition.y);
	platformList.push_back(groundCenter);

	Platform* lvlA1 = new Platform;
	lvlA1->Init(vec2(100, 32), vec2(350, 125));
	lvlA1->mSpriteID = frk.CreateSprite(lvlA1->mSize.x, lvlA1->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlA1->mSpriteID, lvlA1->mPosition.x, lvlA1->mPosition.y);
	platformList.push_back(lvlA1);

	Platform* lvlB1 = new Platform;
	lvlB1->Init(vec2(100, 32), vec2(550, 225));
	lvlB1->mSpriteID = frk.CreateSprite(lvlB1->mSize.x, lvlB1->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlB1->mSpriteID, lvlB1->mPosition.x, lvlB1->mPosition.y);
	platformList.push_back(lvlB1);

	Platform* lvlC1 = new Platform;
	lvlC1->Init(vec2(85, 32), vec2(735, 300));
	lvlC1->mSpriteID = frk.CreateSprite(lvlC1->mSize.x, lvlC1->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlC1->mSpriteID, lvlC1->mPosition.x, lvlC1->mPosition.y);
	platformList.push_back(lvlC1);

	Platform* lvlD1 = new Platform;
	lvlD1->Init(vec2(200, 32), vec2(80, 400));
	lvlD1->mSpriteID = frk.CreateSprite(lvlD1->mSize.x, lvlD1->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlD1->mSpriteID, lvlD1->mPosition.x, lvlD1->mPosition.y);
	platformList.push_back(lvlD1);


	Platform* lvlD3 = new Platform;
	lvlD3->Init(vec2(200, 32), vec2(950, 400));
	lvlD3->mSpriteID = frk.CreateSprite(lvlD3->mSize.x, lvlD3->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlD3->mSpriteID, lvlD3->mPosition.x, lvlD3->mPosition.y);
	platformList.push_back(lvlD3);

	MovingPlatform* mPlat = new MovingPlatform;
	mPlat->Init(vec2(100, 32), vec2(500, 400));
	mPlat->mStopPositionMin = vec2(300, 400);
	mPlat->mStopPositionMax = vec2(750, 400);
	mPlat->mSpriteID = frk.CreateSprite(mPlat->mSize.x, mPlat->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(mPlat->mSpriteID, mPlat->mPosition.x, mPlat->mPosition.y);
	platformList.push_back(mPlat);
}

void UpdatePlatforms(float timeDelta)
{
	for (PlatIt it = platformList.begin(); it != platformList.end(); it++)
	{
		MovingPlatform* mp = dynamic_cast<MovingPlatform*>((*it));
		if (mp)
		{
			mp->Update(timeDelta);
			frk.MoveSprite(mp->mSpriteID, mp->mPosition.x, mp->mPosition.y);
		}
	}
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

	if (frk.IsKeyDown(A) && player.mOnGround)
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

	if (frk.IsKeyDown(D) && player.mOnGround)
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
		frk.DrawSprite((*it)->mSpriteID);
	}
}