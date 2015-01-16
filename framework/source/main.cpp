#include <iostream>
#include "Framework.h"

void InputHandling(glm::vec4& position, bool& quit);

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

	fk.CreateSprite(".\\resources\\images\\lobo.png", 500, 500);
	glm::vec4 spritePosition = vec4(MNF::Globals::SCREEN_WIDTH * .5, MNF::Globals::SCREEN_HEIGHT * .5, 0, 1);
	fk.MoveSprite(spritePosition);

	do
	{
		fk.ClearScreen();
		InputHandling(spritePosition, quit);
		
		fk.DrawSprite();

		
	} while (fk.FrameworkUpdated() && !quit);

	fk.Shutdown();
	return 0;
}

void InputHandling(glm::vec4& position, bool& quit)
{
	if (fk.IsKeyPressed(GLF::W))
	{
		position += glm::vec4(0, .1, 0, 0);
		fk.MoveSprite(position);
	}
	if (fk.IsKeyPressed(GLF::S))
	{
		position -= glm::vec4(0, .1, 0, 0);
		fk.MoveSprite(position);
	}
	if (fk.IsKeyPressed(GLF::A))
	{
		position -= glm::vec4(.1, 0, 0, 0);
		fk.MoveSprite(position);
	}
	if (fk.IsKeyPressed(GLF::D))
	{
		position += glm::vec4(.1, 0, 0, 0);
		fk.MoveSprite(position);
	}
	if (fk.IsKeyPressed(GLF::ESC))
	{
		quit = true;
	}
}