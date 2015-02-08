#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"
#include <string>

#include <time.h>

//#include "Sprite.h"
#include "Framework.h"




//#include "pugixml\pugixml.hpp"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

GLFWwindow* window;
double totalTime;
double deltaTime;

void Initialize();
void Destroy();
void HandleUI(Animation& a);
void ResetDeltaTime();

//debug
int animFrame = 1;
Font font;

Framework frk;

int main()
{
	frk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Framework Test");

	unsigned int s = frk.CreateSprite(50, 50, ".\\resources\\images\\testTexture.png", true);
	frk.MoveSprite(s, MNF::Globals::SCREEN_WIDTH * .5, MNF::Globals::SCREEN_HEIGHT * .5);

	unsigned int animId = frk.CreateAnimation(200, 200, ".\\resources\\images\\smurf_sprite.xml");
	frk.MoveAnimation(animId, 300,300);

	
	do{
		frk.ClearScreen();

		frk.DrawSprite(s);

		frk.DrawText("Foo to the Bar...", 200, 200);

		frk.DrawText("1234567890", 200, 100);
		
		frk.DrawAnimation(animId);

	} while (frk.UpdateFramework());

	frk.Shutdown();

	return 0; 
}