#include "Globals.h"
#include <string>

#include <time.h>
#include <vector>

#include "Framework.h"
#include "Player.h"
#include "Platform.h"

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


float playerMoveSpeed = 100;
float playerJumpVelocity = 1000;

Player player;
uint ground;

std::vector<Platform> platformList;

void Initialize();
void Destroy();
void HandleUI(uint a_spriteID);
void ResetDeltaTime();
bool IsPlayerCollided();
bool RelDiff(float lhs, float rhs);
void LoadPlatforms();

Font font;

Framework frk;

typedef unsigned int uint;

int main()
{


	frk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Framework Test");
	frk.SetBackgroundColor(.482, .698, 1, 1);

	//set the ground level
	uint ground = frk.CreateSprite(1200, 32, ".\\resources\\images\\platform_long.png", true);


	//frk.SetSpriteUV(ground, 0, 388, 192, 420);
	frk.MoveSprite(ground, 512, 15);
	//frk.SetSpriteScale(ground, 1200, 32);

	//uint platform = frk.CreateSprite(192, 32, ".\\resources\\images\\platform_long.png", true);
	//frk.MoveSprite(platform, 800, 200);
	Platform platform;
	platform.Init(vec2(192, 32), vec2(800, 150));
	platform.mSpriteID = frk.CreateSprite(platform.mSize.x, platform.mSize.y, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(platform.mSpriteID, platform.mPosition.x, platform.mPosition.y);

	player.Init(vec2(100, 100), vec2(100, 115));
	player.mSpriteID = frk.CreateAnimation(player.mSize.x, player.mSize.y, ".\\resources\\images\\smurf_sprite.xml");
	frk.MoveAnimation(player.mSpriteID, player.mPosition.x, player.mPosition.y);

	 

	do{
		float deltaTime = frk.GetDeltaTime();
		frk.ClearScreen();

		frk.DrawText("I am a User Interface", SCREEN_WIDTH * .5, SCREEN_HEIGHT * .95);

		frk.DrawSprite(ground);

		frk.DrawSprite(platform.mSpriteID);

		if (player.mVelocity.y > 0 || !player.IsCollided(frk.GetSprite(ground)))
			player.mVelocity.y -= 10;
		else
			player.mVelocity.y = 0;

		if (player.mVelocity.x > 0)
		{
			player.mVelocity.x -= 5;
			frk.SetAnimationState(player.mSpriteID, "walk");
		}
		else if (player.mVelocity.x < 0)
		{
			player.mVelocity.x += 5;
			frk.SetAnimationState(player.mSpriteID, "walk");
		}
		else if (RelDiff(player.mVelocity.x, 0.0f) <= TOLERANCE)
		{
			player.mVelocity.x = 0;
			frk.SetAnimationState(player.mSpriteID, "idle");
		}

		//check collision with platform
		if (player.NewIsCollided(platform))
		{
			//hit bottom or side
			if (player.mColliderBoxMax.y > platform.mColliderBoxMin.y)
			{
				//hit bottom
				std::cout << "hit bottom of platform\n";
			}
			if (player.mColliderBoxMax.x > platform.mColliderBoxMin.x)
			{
				std::cout << "hit left of platform\n";
			}
			if (player.mColliderBoxMin.x < platform.mColliderBoxMax.x)
			{
				std::cout << "hit right side of platform\n";
			}
		}


		frk.MoveAnimation(player.mSpriteID, player.mPosition.x += player.mVelocity.x * deltaTime,
			player.mPosition.y += player.mVelocity.y * deltaTime);
		player.UpdateCollider();

		frk.DrawAnimation(player.mSpriteID);

		HandleUI(ground);

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
	Platform groundLeft;
	groundLeft.Init(vec2(350, 32), vec2(350, 15));
	groundLeft.mSpriteID = frk.CreateSprite(350, 32, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(groundLeft.mSpriteID, groundLeft.mPosition.x, groundLeft.mPosition.y);
}

void HandleUI(uint a_spriteID)
{

	if (frk.IsKeyDown(SPACE) && player.IsCollided(frk.GetSprite(ground)))
	{
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