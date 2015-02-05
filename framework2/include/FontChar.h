#include "glm\glm.hpp"

typedef unsigned int uint;
class FontChar
{
public:
	glm::vec4 position;
	uint ID;
	glm::vec2 fontSheetLocation;
	uint width, height;
	float xOffset, yOffset;
	float xAdvance;
	//left, top, right, bottom
	glm::vec4 UV;
	
	FontChar()
	{
		ID = 0;
		width = 0;
		height = 0;
		xOffset = 0.0f;
		yOffset = 0.0f;
		xAdvance = 0.0f;
	}

private:

};