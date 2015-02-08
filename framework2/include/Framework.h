#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "Globals.h"
#include "Sprite.h"
#include "FontManager.h"
#include "Animation.h"

#include <vector>

class Framework
{
public:
	void Initialize(const int screenWidth, const int screenHeight, const char* title);
	void Shutdown();
	bool UpdateFramework();


	unsigned int CreateSprite(const float width, const float height, const char* texture, const bool isCentered);
	void MoveSprite(unsigned int spriteID, const float xPosition, const float yPosition);
	void DrawSprite(unsigned int spriteID);

	unsigned int CreateAnimation(const float width, const float height, const char* spriteSheetDataFile);
	void MoveAnimation(unsigned int animationID, float xPosition, float yPosition);
	void DrawAnimation(unsigned int animationID);

	void DrawText(const char* text, const float xPosition, float yPosition);

	/*
	Set backgound color to given RGBA values.  Values are expected to be from 0 to 1.
	*/
	void SetBackgroundColor(const float r, const float g, const float b, const float a);

	/*
	Call beginning of main game loop to clear the screen each frame.
	*/
	void ClearScreen();

	double GetDeltaTime();
	double GetTotalTime();
private:
	GLFWwindow* mWindow;
	glm::vec4 mBackgroundColor;
	double mTotalTime;
	double mDeltaTime;
	std::vector<Sprite*> mSpriteList;
	std::vector<Animation*> mAnimationList;
	FontManager mFontMan;

	void ResetDeltaTime();
};

#endif