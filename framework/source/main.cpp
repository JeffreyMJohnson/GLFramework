#include <iostream>
#include "Framework.h"



GLF::Framework fk;
int main()
{
	float hWidth = (MNF::Globals::SCREEN_WIDTH * .5);
	float hHeight = MNF::Globals::SCREEN_HEIGHT * .5;

	if (fk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Game Title", vec4(0,0,0,0)) == 0)
	{	//framework didn't initialize
		return -1;
	}

	fk.CreateSprite(".\\resources\\images\\lobo.png", 500, 500);
	fk.MoveSprite(vec4(MNF::Globals::SCREEN_WIDTH * .5, MNF::Globals::SCREEN_HEIGHT * .5, 0, 1));

	do
	{
		fk.ClearScreen();
		fk.DrawSprite();

		
	} while (fk.FrameworkUpdated());

	fk.Shutdown();
	return 0;
}