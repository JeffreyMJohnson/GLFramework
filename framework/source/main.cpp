#include <iostream>
#include "Framework.h"

#include "Player.h"


GLF::Framework fk;
int main()
{


	if (fk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Game Title", vec4(0,1,0,0)) == 0)
	{	//framework didn't initialize
		return -1;
	}

	do
	{


		fk.ClearScreen();
	} while (fk.FrameworkUpdated());

	fk.Shutdown();
	return 0;
}