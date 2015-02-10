#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"
#include <string>

#include <time.h>

//#include "Sprite.h"
#include "Framework.h"

struct vec2
{
	float x = 0.0f;
	float y = 0.0f;
};

//#include "pugixml\pugixml.hpp"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

GLFWwindow* window;
double totalTime;
double deltaTime;
bool quit = false;

float gravity = 35	;

uint player;
vec2 playerPosition;
vec2 playerSize;
vec2 playerVelocity;
float playerMoveSpeed = 100;
float playerJumpVelocity = 1000;
int playerDirection = 1;
bool playerOnGround = false;
bool playerIsJumping = false;

void Initialize();
void Destroy();
void HandleUI();
void ResetDeltaTime();
bool IsPlayerCollided();

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
	
	playerPosition.x = 100;
	playerPosition.y = 115;
	playerSize.x = 100;
	playerSize.y = 100;
	playerVelocity.x = 0;
	playerVelocity.y = 0;
	player = frk.CreateAnimation(playerSize.x, playerSize.y, ".\\resources\\images\\smurf_sprite.xml");
	frk.MoveAnimation(player, playerPosition.x, playerPosition.y);

	
	do{
		float deltaTime = frk.GetDeltaTime();
		frk.ClearScreen();

		frk.DrawSprite(ground);

		frk.DrawSprite(platform);

		if (!IsPlayerCollided())
			playerPosition.y -= gravity * deltaTime;

		frk.MoveAnimation(player, playerPosition.x += playerVelocity.x * deltaTime, 
			playerPosition.y += playerVelocity.y * deltaTime);
		frk.DrawAnimation(player);

		HandleUI();

	} while (frk.UpdateFramework() && !quit);

	frk.Shutdown();

	return 0; 
}

bool IsPlayerCollided()
{
	if (playerPosition.y - (playerSize.y * .5) < 16)
	{
		frk.MoveAnimation(player, 32, playerPosition.y);
		playerIsJumping = false;
		return true;
	}
	return false;
}

void HandleUI()
{
	if (frk.IsKeyDown(ESC))
	{
		quit = true;
	}

	if (frk.IsKeyDown(A))
	{
		if (playerDirection == 1)
		{
			playerDirection = -1;
			frk.AnimationFlipDirection(player);
			
		}
			frk.SetAnimationState(player, "walk");
		//playerPosition.x -= playerMoveSpeed * frk.GetDeltaTime();
			playerVelocity.x = playerMoveSpeed * playerDirection;
		//frk.MoveAnimation(player, playerPosition.x, playerPosition.y);
		return;
	}
	
	if (frk.IsKeyDown(D))
	{
		if (playerDirection == -1)
		{
			playerDirection = 1;
			frk.AnimationFlipDirection(player);
			
		}
frk.SetAnimationState(player, "walk");
		//playerPosition.x += playerMoveSpeed * playerDirection * frk.GetDeltaTime();
playerVelocity.x = playerMoveSpeed * playerDirection;
		//frk.MoveAnimation(player, playerPosition.x, playerPosition.y);
		
		return;
	}

	if (frk.IsKeyDown(SPACE))
	{
		if (!playerOnGround)
		{
			playerIsJumping = true;
			
		}
		playerVelocity.y = playerJumpVelocity;
	}
	frk.SetAnimationState(player, "idle");
	playerVelocity.x = 0;
	playerVelocity.y = 0; 

}