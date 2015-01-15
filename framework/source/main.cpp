#include <iostream>
#include "Framework.h"

#include "Player.h"


GLF::Framework fk;
int main()
{
	float hWidth = (MNF::Globals::SCREEN_WIDTH * .5);
	float hHeight = MNF::Globals::SCREEN_HEIGHT * .5;

	if (fk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Game Title", vec4(0,0,0,0)) == 0)
	{	//framework didn't initialize
		return -1;
	}
	Player player;
	player.Initialize(vec4(hWidth, hHeight, 0, 1), vec4(1, 1, 1, 1), fk.GetShaderProgram());

	do
	{
		fk.ClearScreen();
		player.Draw(fk.GetShaderMVP(), fk.GetOrtho());

		
	} while (fk.FrameworkUpdated());

	fk.Shutdown();
	return 0;
}