
#include "Font.h"
#include "Sprite.h"

#include <string>


class FontManager
{
public:
	Font mFont;
	Sprite mCharSprite;

	const char* text = "Foo Bar";
	glm::vec2 startPosition = glm::vec2(200, 200);
	float scale = 1;

	FontManager()
	{

	}

	

	void Draw()
	{
		std::string s(text);
		glm::vec2 cursorPosition = glm::vec2(startPosition.x, startPosition.y);
		
		for (std::string::iterator it = s.begin(); it != s.end(); it++)
		{
			mCharSprite.translation = glm::vec3(cursorPosition, 0);
			char currentChar = *it;
			FontChar chInfo = mFont.Chars[currentChar];
			mCharSprite.scale = glm::vec3(chInfo.width * scale, chInfo.height * scale, 1);
			mCharSprite.UpdateTransform();
			mCharSprite.SetUV(chInfo.UV.x, chInfo.UV.y, chInfo.UV.z, chInfo.UV.w);
			mCharSprite.Draw();
			cursorPosition += glm::vec2(chInfo.xAdvance, 0) * scale;
		}
	}


	unsigned int Initialize(const char* fontPath, const char* descriptionFile)
	{
		mFont.Init(fontPath, descriptionFile);
		mCharSprite.Initialize(0, 0, (mFont.path + mFont.imageFileName).c_str(), false);
		return 1;
	}

	void LoadFont(const char* fontPath, const char* descriptionFile)
	{
		
	}
};