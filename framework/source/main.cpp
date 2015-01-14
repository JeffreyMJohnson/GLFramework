#include <iostream>
#include "Framework.h"
#include "Globals.h"

typedef GLF::Framework Frwk;

int main()
{
	if (Frwk::Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Game Title") == 0)
	{	//framework didn't initialize
		return -1;
	}

	Frwk::Shutdown();
	return 0;
}