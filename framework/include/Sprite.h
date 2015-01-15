
#include "glm\glm.hpp"
#include "SOIL\SOIL.h"
#include <iostream>
#include "Vertex.h"

typedef glm::vec2 vec2;
typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef unsigned int uint;
namespace GLF
{

	class Sprite
	{
	public:
		Sprite()
		{

		}

		void Update()
		{

		}

		void Draw();

		uint Initialize(const char* textureName, int width, int height, const vec2 position, const vec4& color)
		{
			int bpp;
			this->width = width;
			this->height = height;
			this->position = position;
			LoadModelVertices();
			LoadModelUVs();
			verticesBuffer = new Vertex[4];
			textureID = LoadTexture(textureName, width, height, bpp);

			return 0;
		}

		uint GetTextureID()
		{
			return textureID;
		}

		Vertex* GetModelVertices()
		{
			return verticesBuffer;
		}

		void UpdateVertices()
		{
			for (int i = 0; i < 4; i++)
			{
				verticesBuffer[i].fPositions[0] = modelVertices[i].x + position.x;
				verticesBuffer[i].fPositions[1] = modelVertices[i].y + position.y;
				verticesBuffer[i].fColors[0] = 1;
				verticesBuffer[i].fColors[1] = 1;
				verticesBuffer[i].fColors[2] = 1;
				verticesBuffer[i].fColors[3] = 1;
				verticesBuffer[i].fUVs[0] = modelUVs[i].x;
				verticesBuffer[i].fUVs[1] = modelUVs[i].y;
			}

		}
	private:
		uint spriteID;
		int width;
		int height;
		uint textureID;
		vec2 position;
		std::vector<glm::vec3> modelVertices;
		std::vector<glm::vec2> modelUVs;
		Vertex* verticesBuffer;

		void LoadModelVertices()
		{
			float hWidth = .5 * width;
			float hHeight = .5 * height;
			modelVertices.push_back(vec3(-hWidth, hHeight, 0));
			modelVertices.push_back(vec3(hWidth, hHeight, 0));
			modelVertices.push_back(vec3(hWidth, -hHeight, 0));
			modelVertices.push_back(vec3(-hWidth, -hHeight, 0));
		}

		void LoadModelUVs()
		{
			modelUVs.push_back(vec2(0, 1));
			modelUVs.push_back(vec2(1, 1));
			modelUVs.push_back(vec2(1,0));
			modelUVs.push_back(vec2(0,0));
		}

		unsigned int LoadTexture(const char* a_pFilename, int & a_iWidth, int & a_iHeight, int & a_iBPP)
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