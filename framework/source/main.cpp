#include <iostream>
//#include "Framework.h"
//#include "Globals.h"
#include "Application.h"

//typedef GLF::Framework Frwk;

int main()
{
	Application appInstance;
	appInstance.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "My Game Title");
	appInstance.SetBackgroundColor(glm::vec4(.2, .3, .3, 1));
	//uint spriteId = appInstance.mySprite.Initialize(".\\resources\\images\\lobo.png", 500, 500, vec2(MNF::Globals::SCREEN_WIDTH * .5, MNF::Globals::SCREEN_HEIGHT * .5), vec4(1, 1, 1, 1));

	do
	{
		appInstance.ClearScreen();
		//appInstance.DrawSprite();

	} while (appInstance.FrameworkUpdate());


	appInstance.Shutdown();

	return 0;
}