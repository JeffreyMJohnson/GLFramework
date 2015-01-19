#include <iostream>
#include "Framework.h"

void InputHandling(const uint spriteID, glm::vec4& position, bool& quit);
void Update();

GLF::Framework fk;
/*
TODO::
	implement ability to have more than one hard coded sprite;
*/
int currentAnimIndex = 0;
std::vector<glm::vec4> animatedUVRecs;
uint mySprite;
uint sprite2;
int main()
{
	float hWidth = (MNF::Globals::SCREEN_WIDTH * .5);
	float hHeight = MNF::Globals::SCREEN_HEIGHT * .5;
	bool quit = false;

	animatedUVRecs.push_back(glm::vec4(0, 1, .25, .75));
	animatedUVRecs.push_back(glm::vec4(.25, 1, .5, .75));


	if (fk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Game Title", vec4(0,0,0,0)) == 0)
	{	//framework didn't initialize
		return -1;
	}


	mySprite = fk.CreateSprite(".\\resources\\images\\testTexture.png", 100, 100, animatedUVRecs[currentAnimIndex]);
	//sprite2 = fk.CreateSprite(".\\resources\\images\\lobo.png", 250, 250);

	int sheetWidth = 256;
	int sheetHeight = 256;
	GLF::CharDescriptor ch = fk.charSetDesc.Chars['n'];
	glm::vec4 charRect;
	//charRect.x = ch.x;//minX
	//charRect.y = ch.y + ch.height;//maxY
	//charRect.z = ch.x + ch.width;//maxX
	//charRect.w = ch.y;//minY
	
	//top left origin?
	float left = ch.x;
	float top = sheetHeight - (ch.y);//invert origin?
	float right = left + ch.width;
	float bottom = sheetHeight - (ch.y + ch.height);//invert origin

	//normalize
	vec4 charUVs = vec4(
		left / sheetWidth,
		top/ sheetHeight,
		right / sheetWidth,
		bottom / sheetHeight);
	sprite2 = fk.CreateSprite(".\\resources\\fonts\\arial_0.png", ch.width, ch.height, charUVs);

	glm::vec4 spritePosition = vec4(MNF::Globals::SCREEN_WIDTH * .25, MNF::Globals::SCREEN_HEIGHT * .25, 0, 1);
	fk.MoveSprite(mySprite, spritePosition);

	glm::vec4 spritePosition2 = vec4(MNF::Globals::SCREEN_WIDTH * .75, MNF::Globals::SCREEN_HEIGHT * .75, 0, 1);
	fk.MoveSprite(sprite2, spritePosition2);

	do
	{
		fk.ClearScreen();
		InputHandling(mySprite, spritePosition, quit);
		InputHandling(sprite2, spritePosition2, quit);

		Update();

		fk.DrawSprite(mySprite);		
		fk.DrawSprite(sprite2);
		//fk.DrawString();

		
	} while (fk.FrameworkUpdated() && !quit);

	fk.Shutdown();
	return 0;
}



void Update()
{
	if (currentAnimIndex == 0)
		currentAnimIndex = 1;
	else
		currentAnimIndex = 0;
	fk.SetSpriteUVRectangle(mySprite, animatedUVRecs[currentAnimIndex]);
}

void InputHandling(const uint spriteID, glm::vec4& position, bool& quit)
{
	if (fk.IsKeyPressed(GLF::W))
	{
		position += glm::vec4(0, .1, 0, 0);
		fk.MoveSprite(spriteID, position);
	}
	if (fk.IsKeyPressed(GLF::S))
	{
		position -= glm::vec4(0, .1, 0, 0);
		fk.MoveSprite(spriteID, position);
	}
	if (fk.IsKeyPressed(GLF::A))
	{
		position -= glm::vec4(.1, 0, 0, 0);
		fk.MoveSprite(spriteID, position);
	}
	if (fk.IsKeyPressed(GLF::D))
	{
		position += glm::vec4(.1, 0, 0, 0);
		fk.MoveSprite(spriteID, position);
	}
	if (fk.IsKeyPressed(GLF::ESC))
	{
		quit = true;
	}
}