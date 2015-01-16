#include <iostream>
#include "Framework.h"

void InputHandling(const uint spriteID, glm::vec4& position, bool& quit);

GLF::Framework fk;
/*
TODO::
	implement ability to have more than one hard coded sprite;
*/

int main()
{
	float hWidth = (MNF::Globals::SCREEN_WIDTH * .5);
	float hHeight = MNF::Globals::SCREEN_HEIGHT * .5;
	bool quit = false;

	

	if (fk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Game Title", vec4(0,0,0,0)) == 0)
	{	//framework didn't initialize
		return -1;
	}

	const uint mySprite = fk.CreateSprite(".\\resources\\images\\lobo2.png", 250, 250);
	const uint sprite2 = fk.CreateSprite(".\\resources\\images\\lobo.png", 250, 250);

	glm::vec4 spritePosition = vec4(MNF::Globals::SCREEN_WIDTH * .25, MNF::Globals::SCREEN_HEIGHT * .25, 0, 1);
	fk.MoveSprite(mySprite, spritePosition);

	glm::vec4 spritePosition2 = vec4(MNF::Globals::SCREEN_WIDTH * .75, MNF::Globals::SCREEN_HEIGHT * .75, 0, 1);
	fk.MoveSprite(sprite2, spritePosition2);

	do
	{
		fk.ClearScreen();
		InputHandling(mySprite, spritePosition, quit);
		InputHandling(sprite2, spritePosition2, quit);
		
		fk.DrawSprite(mySprite);
		fk.DrawSprite(sprite2);

		
	} while (fk.FrameworkUpdated() && !quit);

	fk.Shutdown();
	return 0;
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