#include <iostream>
//#include "Framework.h"
//#include "Globals.h"
#include "Application.h"

//typedef GLF::Framework Frwk;

int main()
{
	Application appInstance;
	appInstance.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "My Game Title");



	appInstance.Shutdown();

	return 0;
}