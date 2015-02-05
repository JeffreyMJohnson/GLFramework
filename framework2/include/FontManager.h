
#include "Font.h"
#include "Sprite.h"




class FontManager
{
public:
	Font mFont;
	Sprite mCharSprite;

	
	glm::vec2 startPosition = glm::vec2(200, 200);

	FontManager()
	{

	}

	void Draw()
	{
		const char* text = "Foo";
		glm::vec2 cursorPosition = glm::vec2(startPosition.x, startPosition.y);
		mCharSprite.translation = glm::vec3(cursorPosition, 0);
		char currentChar = *text;
		FontChar chInfo = mFont.Chars[currentChar];
		mCharSprite.translation += glm::vec3(chInfo.xOffset,0, 0);
		mCharSprite.scale = glm::vec3(chInfo.width, chInfo.height, 1);
		mCharSprite.UpdateTransform();
		mCharSprite.SetUV(chInfo.UV.x, chInfo.UV.y, chInfo.UV.z, chInfo.UV.w);
		mCharSprite.Draw();

		cursorPosition += glm::vec2(chInfo.xAdvance, 0);
		mCharSprite.translation = glm::vec3(cursorPosition, 0);
		currentChar = *(++text);
		chInfo = mFont.Chars[currentChar];
		mCharSprite.scale = glm::vec3(chInfo.width, chInfo.height, 1);
		mCharSprite.translation += glm::vec3(chInfo.xOffset, 0, 0);
		mCharSprite.UpdateTransform();
		mCharSprite.SetUV(chInfo.UV.x, chInfo.UV.y, chInfo.UV.z, chInfo.UV.w);
		mCharSprite.Draw();
	}


	unsigned int Initialize(const char* fontPath, const char* descriptionFile)
	{
		mFont.Init(fontPath, descriptionFile);
		mCharSprite.Initialize(0, 0, (mFont.path + mFont.imageFileName).c_str());
		return 1;
	}

	void LoadFont(const char* fontPath, const char* descriptionFile)
	{
		
	}
};