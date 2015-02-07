#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"
#include <string>

#include <time.h>

//#include "Sprite.h"
#include "Framework.h"



#include "Animation.h"
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

	//Initialize();
	//CreateShaderProgram();
	/*Sprite s;
	s.Initialize(25, 25, ".\\resources\\images\\testTexture.png", true);
	s.translation = glm::vec3(SCREEN_WIDTH * .5, SCREEN_HEIGHT * .5, 0);
	s.UpdateTransform();*/
	//s.SetUV(0, 0, .25, .25);

	unsigned int s = frk.CreateSprite(50, 50, ".\\resources\\images\\testTexture.png", true);
	frk.MoveSprite(s, MNF::Globals::SCREEN_WIDTH * .5, MNF::Globals::SCREEN_HEIGHT * .5);


	//work on font
	//FontManager fm;
	//fm.Initialize(".\\resources\\fonts\\", "arial.fnt");

	glm::vec2 currentFrame(0,2);
	glm::vec2 sheetSize(50, 50);
	glm::vec2 spriteCount(4, 4);
	glm::vec2 frameSize(sheetSize.x / spriteCount.x, sheetSize.y / spriteCount.y);
	//normalize
	//s.SetUV(
	//	(currentFrame.x * frameSize.x) / sheetSize.x,
	//	(currentFrame.y * frameSize.y) / sheetSize.y,
	//	((currentFrame.x * frameSize.x) + frameSize.x) / sheetSize.x,
	//	((currentFrame.y * frameSize.y) + frameSize.y) / sheetSize.y
	//	);

	//work on animation
	Animation anim;
	deltaTime = 0;
	totalTime = 0;



	
	do{
		/*glClearColor(.5, .5, .5, 1);
		glClear(GL_COLOR_BUFFER_BIT);*/
		frk.ClearScreen();

		//ResetDeltaTime();
		
		
		//s.Update();
		//s.Draw();

		frk.DrawSprite(s);

		frk.DrawText("Foo to the Bar...", 200, 200);

		frk.DrawText("1234567890", 200, 100);
		anim.Update(frk.GetDeltaTime());
		anim.Draw();

		//HandleUI(anim);

		//glfwSwapBuffers(window);
		//glfwPollEvents();
	} while (frk.UpdateFramework());

	Destroy();

	return 0; 
}

void ResetDeltaTime()
{
	deltaTime = glfwGetTime();
	totalTime += deltaTime;
	glfwSetTime(0);
}

void Initialize()
{
	glfwInit();
	//create the window
	//windowed
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Bitches!", nullptr, nullptr);
	//full screen
	//window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Bitches!", glfwGetPrimaryMonitor(), nullptr);

	//make window active
	glfwMakeContextCurrent(window);

	//force GLEW to use a modern OpenGL method for checking if function is available
	glewExperimental = GL_TRUE;
	//initialize GLEW
	glewInit();

	totalTime = 0.0;
	deltaTime = 0.0;
}

void Destroy()
{
	glfwTerminate();
}



//void HandleUI(Animation& a)
//{
//	if (glfwGetKey(window, GLFW_KEY_A))
//	{
//		//s.translation += glm::vec3(-.1, 0, 0);
//		a.mSprite.translation += glm::vec3(-.1, 0, 0);
//		//s.UpdateTransform();
//		a.mSprite.UpdateTransform();
//		a.mIsWalking = true;
//		if (a.mDirection == 1)
//			a.SwitchDirection();
//		return;
//	}
//	if (glfwGetKey(window, GLFW_KEY_D))
//	{
//		//s.translation += glm::vec3(.1, 0, 0);
//		a.mSprite.translation += glm::vec3(.1, 0, 0);
//		
//		//s.UpdateTransform();
//		a.mSprite.UpdateTransform();
//		a.mIsWalking = true;
//		if (a.mDirection == -1)
//			a.SwitchDirection();
//		return;
//	}
//	if (glfwGetKey(window, GLFW_KEY_W))
//	{
//		//s.translation += glm::vec3(0, .1, 0);
//		//s.UpdateTransform();
//	}
//	if (glfwGetKey(window, GLFW_KEY_S))
//	{
//		//s.translation += glm::vec3(0, -.1, 0);
//		//s.UpdateTransform();
//	}
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
//	{
//		glfwSetWindowShouldClose(window, GL_TRUE);
//	}
//	/*if (glfwGetKey(window, GLFW_KEY_TAB))
//	{
//		if (animFrame == 1)
//		{
//			animFrame = 2;
//			s.SetUV(.5, .5, .75, .75);
//		}
//		else
//		{
//			animFrame = 1;
//			s.SetUV(.25, .25, .5, .5);
//		}
//	}*/
//	a.mIsWalking = false;
//}