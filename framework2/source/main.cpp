#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"
#include <string>

#include <time.h>

//#include "Sprite.h"
#include "Framework.h"
#include "Player.h"

#define Abs(x) ((x) < 0 ? -(x) : (x))
#define Max(a, b) ((a) > (b) ? (a) : (b))

struct vec2
{
	float x = 0.0f;
	float y = 0.0f;
};

//#include "pugixml\pugixml.hpp"

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


void Initialize();
void Destroy();
void HandleUI(uint a_spriteID);
void ResetDeltaTime();
bool IsPlayerCollided();
bool RelDiff(float lhs, float rhs);

//debug
int animFrame = 1;
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

	uint platform = frk.CreateSprite(192, 32, ".\\resources\\images\\platform_long.png", true);
	frk.MoveSprite(platform, 800, 200);


	player.mSpriteID = frk.CreateAnimation(player.mSize.x, player.mSize.y, ".\\resources\\images\\smurf_sprite.xml");
	frk.MoveAnimation(player.mSpriteID, player.mPosition.x, player.mPosition.y);

	


	do{
		float deltaTime = frk.GetDeltaTime();
		frk.ClearScreen();

		frk.DrawText("I am a User Interface", SCREEN_WIDTH * .5, SCREEN_HEIGHT * .95);

		frk.DrawSprite(ground);

		frk.DrawSprite(platform);

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


		frk.MoveAnimation(player.mSpriteID, player.mPosition.x += player.mVelocity.x * deltaTime,
			player.mPosition.y += player.mVelocity.y * deltaTime);
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