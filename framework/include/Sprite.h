
#include "glm\glm.hpp"
#include "SOIL\SOIL.h"
#include <iostream>

typedef glm::vec2 vec2;
typedef glm::vec4 vec4;
typedef unsigned int uint;
namespace GLF
{
	
	class Sprite
	{
	public:
		Sprite()
		{

		}

		uint CreateSprite(const char* textureName, const int width, const int height, const vec2 position, const vec4& color)
		{

		}
	private:
		uint spriteID;
		int width;
		int height;

		unsigned int loadTexture(const char* a_pFilename, int & a_iWidth, int & a_iHeight, int & a_iBPP)
		{
			unsigned int uiTextureID = 0;
			//check file exists
			if (a_pFilename != nullptr)
			{
				//read in image data from file
				unsigned char* pImageData = SOIL_load_image(a_pFilename, &a_iWidth, &a_iHeight, &a_iBPP, SOIL_LOAD_AUTO);

				//check for successful read
				if (pImageData)
				{
					//create opengl texture handle
					uiTextureID = SOIL_create_OGL_texture(pImageData, a_iWidth, a_iHeight, a_iBPP,
						SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
					//clear what was read in from file now that it is stored in the handle
					SOIL_free_image_data(pImageData);
				}

				//check for errors
				if (uiTextureID == 0)
				{
					std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
				}
				return uiTextureID;
			}
		}
	};

	};
}