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

enum KEY_CODE
{
	A = GLFW_KEY_A,
	S = GLFW_KEY_S,
	D = GLFW_KEY_D,
	W = GLFW_KEY_W,
	SPACE = GLFW_KEY_SPACE,
	ESC = GLFW_KEY_ESCAPE
};

class Framework
{
public:
	void Initialize(const int screenWidth, const int screenHeight, const char* title);
	void Shutdown();
	bool UpdateFramework();


	uint CreateSprite(const float width, const float height, const char* texture, const bool isCentered);
	void SetSpriteUV(const uint spriteID, const float minX, const float minY, const float maxX, const float maxY);
	void SetSpriteScale(uint spriteId, float scaleX, float scaleY);
	void MoveSprite(uint spriteID, const float xPosition, const float yPosition);
	void DrawSprite(uint spriteID);
	Sprite& GetSprite(uint spriteID);

	uint CreateAnimation(const float width, const float height, const char* spriteSheetDataFile);
	void AnimationFlipDirection(uint animationId);
	void SetAnimationState(const uint animationId, const char* a_state);
	void MoveAnimation(uint animationID, float xPosition, float yPosition);
	void DrawAnimation(uint animationID);

	void DrawText(const char* text, const float xPosition, float yPosition);

	/*
	Set backgound color to given RGBA values.  Values are expected to be from 0 to 1.
	*/
	void SetBackgroundColor(const float r, const float g, const float b, const float a);

	/*
	Call beginning of main game loop to clear the screen each frame.
	*/
	void ClearScreen();

	bool IsKeyDown(KEY_CODE a_key);

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