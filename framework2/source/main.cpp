#include "Globals.h"
#include <string>

#include <time.h>
#include <vector>
#include <stdio.h>

#include "framework/Framework.h"
#include "Player.h"
#include "Platform.h"
#include "MovingPlatform.h"


const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const float DEBUG_TIME_DELTA = 1.0f / 350;

GLFWwindow* window;
double totalTime;
double deltaTime;
bool quit = false;


float playerMoveSpeed = 250;
float playerJumpVelocity = 1000;

Player player;

std::vector<Platform*> platformList;
typedef std::vector<Platform*>::iterator PlatIt;

void Initialize();
void Destroy();
void HandleUI();
void ResetDeltaTime();
bool IsPlayerCollided();
void LoadPlatforms();
void UpdatePlatforms(float timeDelta);
void DrawPlatforms();
void PlayerLogic();
void CheckPlatformCollision();
void ShutDown();

Font font;

Framework frk;

int main()
{


	frk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Framework Test");
	frk.SetBackgroundColor(.482, .698, 1, 1);

	LoadPlatforms();

	//debug
	player.Init(glm::vec2(75, 75), glm::vec2(100, 200));
	//player.Init(glm::vec2(50, 50), glm::vec2(350, 500));
	player.mColliderOffset = glm::vec2(.25, .45);
	player.mSpriteID = frk.CreateAnimation(player.mSize.x, player.mSize.y, ".\\resources\\images\\smurf_sprite.xml");
	frk.MoveAnimation(player.mSpriteID, player.mPosition.x, player.mPosition.y);
	player.UpdateCollider();

	char timerText[256];
	sprintf(timerText, "%f", totalTime);

	do{
		//debug
		deltaTime = frk.GetDeltaTime();
		//deltaTime = DEBUG_TIME_DELTA;
		frk.ClearScreen();

		

		player.Update(deltaTime, platformList);
		frk.SetAnimationState(player.mSpriteID, player.mCurrentAnimationState);
		HandleUI();
		frk.MoveAnimation(player.mSpriteID, player.mPosition.x += player.mVelocity.x * deltaTime,
			player.mPosition.y += player.mVelocity.y * deltaTime);


		frk.DrawText(timerText, SCREEN_WIDTH * .40, SCREEN_HEIGHT * .95);
		
		UpdatePlatforms(deltaTime);
		DrawPlatforms();
		frk.DrawAnimation(player.mSpriteID);
		player.UpdateCollider();
		sprintf(timerText, "%05.1f", frk.GetTotalTime());

	} while (frk.UpdateFramework() && !quit);

	frk.Shutdown();

	ShutDown();

	return 0;
}

void LoadPlatforms()
{

	Platform* groundLeft = new Platform;
	groundLeft->Init(glm::vec2(400, 32), glm::vec2(100, 15));
	groundLeft->mSpriteID = frk.CreateSprite(groundLeft->mSize.x, groundLeft->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(groundLeft->mSpriteID, groundLeft->mPosition.x, groundLeft->mPosition.y);
	platformList.push_back(groundLeft);



	Platform* groundCenter = new Platform;
	groundCenter->Init(glm::vec2(175, 32), glm::vec2(475, 15));
	groundCenter->mSpriteID = frk.CreateSprite(groundCenter->mSize.x, groundCenter->mSize.y, ".\\resources\\images\\platform_short.png", true);
	frk.MoveSprite(groundCenter->mSpriteID, groundCenter->mPosition.x, groundCenter->mPosition.y);
	platformList.push_back(groundCenter);

	Platform* lvlA1 = new Platform;
	lvlA1->Init(glm::vec2(125, 32), glm::vec2(350, 100));
	lvlA1->mSpriteID = frk.CreateSprite(lvlA1->mSize.x, lvlA1->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlA1->mSpriteID, lvlA1->mPosition.x, lvlA1->mPosition.y);
	platformList.push_back(lvlA1);

	Platform* lvlB1 = new Platform;
	lvlB1->Init(glm::vec2(125, 32), glm::vec2(525, 150));
	lvlB1->mSpriteID = frk.CreateSprite(lvlB1->mSize.x, lvlB1->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlB1->mSpriteID, lvlB1->mPosition.x, lvlB1->mPosition.y);
	platformList.push_back(lvlB1);

	Platform* lvlC1 = new Platform;
	lvlC1->Init(glm::vec2(125, 32), glm::vec2(700, 200));
	lvlC1->mSpriteID = frk.CreateSprite(lvlC1->mSize.x, lvlC1->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlC1->mSpriteID, lvlC1->mPosition.x, lvlC1->mPosition.y);
	platformList.push_back(lvlC1);

	Platform* lvlD1 = new Platform;
	lvlD1->Init(glm::vec2(200, 32), glm::vec2(80, 400));
	lvlD1->mSpriteID = frk.CreateSprite(lvlD1->mSize.x, lvlD1->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlD1->mSpriteID, lvlD1->mPosition.x, lvlD1->mPosition.y);
	platformList.push_back(lvlD1);


	Platform* lvlD3 = new Platform;
	lvlD3->Init(glm::vec2(200, 32), glm::vec2(950, 250));
	lvlD3->mSpriteID = frk.CreateSprite(lvlD3->mSize.x, lvlD3->mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(lvlD3->mSpriteID, lvlD3->mPosition.x, lvlD3->mPosition.y);
	platformList.push_back(lvlD3);

	MovingPlatform* mPlat = new MovingPlatform;
	mPlat->Init(glm::vec2(150, 32), glm::vec2(300, 300));
	mPlat->mStopPositionMin = glm::vec2(300, 400);
	mPlat->mStopPositionMax = glm::vec2(750, 300);
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
 		player.mVelocity.y = playerJumpVelocity;
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

void ShutDown()
{
	for (PlatIt it = platformList.begin(); it != platformList.end(); it++)
	{
		delete *it;
	}
	platformList.clear();
}