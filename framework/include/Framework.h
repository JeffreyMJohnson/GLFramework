#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "GL\glew.h"
//#include "GL\wglew.h"

//glew must be included first
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "SOIL\SOIL.h"
#include "pugixml\pugixml.hpp"

#include "Globals.h"
#include "Sprite.h"
#include "Font.h"

#include <fstream>
#include <vector>
#include <string>

typedef glm::vec4 vec4;
typedef unsigned int uint;
typedef unsigned short ushort;

namespace GLF
{
	enum KEY_CODE
	{
		SPACE_BAR = GLFW_KEY_SPACE,
		W = GLFW_KEY_W,
		S = GLFW_KEY_S,
		A = GLFW_KEY_A,
		D = GLFW_KEY_D,
		ESC = GLFW_KEY_ESCAPE
	};

	//holds info for single character of font sprite sheet
	//struct CharDescriptor
	//{
	//	ushort x, y;
	//	ushort width, height;
	//	float xOffset, yOffset;
	//	float xAdvance;
	//	ushort page;

	//	CharDescriptor()
	//	{
	//		x = 0;
	//		y = 0;
	//		width = 0;
	//		height = 0;
	//		xOffset = 0;
	//		yOffset = 0;
	//		xAdvance = 0;
	//		page = 0;
	//	}
	//};

	////holds info for entire character set from font sprite sheet and collection of complete characters indexed by ascii code
	//struct Charset
	//{
	//	ushort lineHeight;
	//	ushort base;
	//	ushort scaleW;
	//	ushort scaleH;
	//	ushort pages;
	//	CharDescriptor Chars[256];
	//};

	class Framework
	{
	public:
		/**
		Initialize GLFramework
		params:
		a_screenWidth - width of the window to create
		a_screenHeight - height of the window to create
		a_title - title of the window to create.
		returns:
		returns -1 if Initialize has no errors else returns 0
		*/
		int Initialize(int a_screenWidth, int a_screenHeight, const char* a_title, vec4 a_backgroundColor)
		{
			if (!glfwInit())
				return 0;
			windowHandle = glfwCreateWindow(a_screenWidth, a_screenHeight, a_title, NULL, NULL);

			if (windowHandle == NULL)
				return 0;
			glfwMakeContextCurrent(windowHandle);

			//init glew
			if (glewInit() != GLEW_OK)
			{
				//glew didn't start so shutdown glfw and return error
				glfwTerminate();
				return 0;
			}

			CreateShaderProgram();

			IDTexture = glGetUniformLocation(shaderProgram, "MVP");
			orthographicProjection = getOrtho(0, MNF::Globals::SCREEN_WIDTH, 0, MNF::Globals::SCREEN_HEIGHT, 0, 100);
			backgroundColor = a_backgroundColor;
			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//ParseFont(".\\resources\\fonts\\arial.fnt");

			//load font sprite sheet
			//int textureWidth = 50;
			//int textureHeight = 50;
			//int textureBPP = 4;
			fontWidth = 25;
			fontHeight = 25;
			fontsSpriteSheet = loadTexture(".\\resources\\fonts\\arial_0.png", fontSheetWidth, fontSheetHeight, fontSheetBPP);
			//LoadFontChars();
			myFont.Init(".\\resources\\fonts\\arial.fnt");
			return -1;

		}

		/**
		Create a 2D sprite from given filename with given width and height.
		returns a unique Sprite ID
		*/
		uint CreateSprite(char* a_fileName, int a_width, int a_height)
		{
			Sprite* newSprite = new Sprite;
			//glGenBuffers(1, &mySprite.uiVBO);
			glGenBuffers(1, &newSprite->uiVBO);

			//mySprite.Initialize(shaderProgram, a_width, a_height);
			newSprite->Initialize(shaderProgram, a_width, a_height);

			int textureWidth = 50;
			int textureHeight = 50;
			int textureBPP = 4;

			//mySprite.uiTextureID = loadTexture(a_fileName, textureWidth, textureHeight, textureBPP);
			newSprite->uiTextureID = loadTexture(a_fileName, textureWidth, textureHeight, textureBPP);
			spriteList.push_back(newSprite);

			//return the sprites index for accessing later without search
			return spriteList.size() - 1;
		}

		uint CreateSprite(char* a_fileName, int a_width, int a_height, glm::vec4& UVCoordinates)
		{
			Sprite* newSprite = new Sprite;
			//glGenBuffers(1, &mySprite.uiVBO);
			glGenBuffers(1, &newSprite->uiVBO);

			//mySprite.Initialize(shaderProgram, a_width, a_height);
			newSprite->Initialize(shaderProgram, a_width, a_height);

			int textureWidth = 50;
			int textureHeight = 50;
			int textureBPP = 4;


			//mySprite.uiTextureID = loadTexture(a_fileName, textureWidth, textureHeight, textureBPP);
			newSprite->uiTextureID = loadTexture(a_fileName, textureWidth, textureHeight, textureBPP);
			newSprite->SetUVCoordinates(UVCoordinates);
			spriteList.push_back(newSprite);

			//return the sprites index for accessing later without search
			return spriteList.size() - 1;
		}

		uint CreateSprite(uint textureID, int a_width, int a_height, glm::vec4& UVCoordinates)
		{
			Sprite* newSprite = new Sprite;
			//glGenBuffers(1, &mySprite.uiVBO);
			glGenBuffers(1, &newSprite->uiVBO);

			//mySprite.Initialize(shaderProgram, a_width, a_height);
			newSprite->Initialize(shaderProgram, a_width, a_height);

			int textureWidth = 50;
			int textureHeight = 50;
			int textureBPP = 4;


			//mySprite.uiTextureID = loadTexture(a_fileName, textureWidth, textureHeight, textureBPP);
			newSprite->uiTextureID = textureID;
			newSprite->SetUVCoordinates(UVCoordinates);
			spriteList.push_back(newSprite);

			//return the sprites index for accessing later without search
			return spriteList.size() - 1;
		}

		void MoveSprite(const uint spriteID, const vec4& a_position)
		{
			Sprite* s = spriteList[spriteID];
			//mySprite.SetPosition(a_position);
			s->SetPosition(a_position);

			//UpdateVBO(mySprite.uiVBO, mySprite.verticesBuffer, 4);
			UpdateVBO(s->uiVBO, s->verticesBuffer, 4);
		}

		void SetSpriteUVRectangle(const uint spriteID, glm::vec4 newUVRectangle)
		{
			spriteList[spriteID]->SetUVCoordinates(newUVRectangle);
			UpdateVBO(spriteList[spriteID]->uiVBO, spriteList[spriteID]->verticesBuffer, 4);
		}

		void DrawString(std::string text, glm::vec4 a_position)
		{
			glm::vec4 currentPosition = a_position;
			
		}

		std::vector<uint> charList;
		void CreateText(std::string text)
		{
			glm::vec4 currentPosition = glm::vec4(MNF::Globals::SCREEN_WIDTH * .1, MNF::Globals::SCREEN_HEIGHT * .6, 0, 1);
			for (std::string::iterator it = text.begin(); it != text.end(); it++)
			{
				uint ch = CreateSprite(fontsSpriteSheet, myFont.Chars[*it].width, myFont.Chars[*it].height, myFont.Chars[*it].UV);
				if (it == text.begin())
				{
					//currentPosition += glm::vec4(myFont.Chars[*it].width * -.5, myFont.Chars[*it].height * .5, 0, 0);
				}
				else
				{
					FontChar current = myFont.Chars[*it];
					FontChar prev = myFont.Chars[*(it - 1)];
					currentPosition += glm::vec4(current.xOffset + prev.xAdvance , 0, 0, 0);
				}
				
				//uint ch = CreateSprite(".\\resources\\fonts\\arial_0.png", 20, 20, myFont.Chars[*it].UV);
				MoveSprite(ch, currentPosition);
				charList.push_back(ch);
			}

		}
			//void DrawChar(char a_char, glm::vec4 position)
			//{
			//	FontChar ch = myFont.Chars[a_char];
			//	uint spriteID = CreateSprite(myFont.fontSpriteSheetTextureID, 20, 20, ch.UV);
			//	MoveSprite(spriteID, position);
			//	//UpdateVBO(s.uiVBO, s.verticesBuffer, 4);
			//	DrawSprite(spriteID);
			//	//glUseProgram(shaderProgram);

			//	////send ortho projection info to shader
			//	//glUniformMatrix4fv(IDTexture, 1, GL_FALSE, orthographicProjection);

			//	////enable vertex array state
			//	//glEnableVertexAttribArray(0);
			//	//glEnableVertexAttribArray(1);
			//	//glEnableVertexAttribArray(2);




			//	////glBindTexture(GL_TEXTURE_2D, mySprite.uiTextureID);
			//	//glBindTexture(GL_TEXTURE_2D, fontChars[a_char].uiTextureID);

			//	////glBindBuffer(GL_ARRAY_BUFFER, mySprite.uiVBO);
			//	//glBindBuffer(GL_ARRAY_BUFFER, fontChars[a_char].uiVBO);

			//	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			//	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
			//	////now we have UVs to worry about, we need to send that info to the graphics card too
			//	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 8));

			//	//// Enable blending
			//	//glEnable(GL_BLEND);
			//	////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//	//glDrawArrays(GL_QUADS, 0, sizeof(Vertex));

			//	//glBindBuffer(GL_ARRAY_BUFFER, 0);
			//	//glBindTexture(GL_TEXTURE_2D, 0);

			//}

			bool FrameworkUpdated()
			{
				if (glfwWindowShouldClose(windowHandle))
				{
					return false;
				}

				if (charList.size() > 0)
				{
					for (uint ch : charList)
					{
						DrawSprite(ch);
					}
				}

				glfwSwapBuffers(windowHandle);

				//poll for and process events
				glfwPollEvents();
				return true;
			}

			void SetBackgroundColor(const vec4 a_color)
			{
				backgroundColor = a_color;
			}

			bool IsKeyPressed(KEY_CODE key)
			{
				return glfwGetKey(windowHandle, key);
			}

			void ClearScreen()
			{
				//glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			void DrawSprite(const uint spriteID)
			{
				glUseProgram(shaderProgram);

				//send ortho projection info to shader
				glUniformMatrix4fv(IDTexture, 1, GL_FALSE, orthographicProjection);

				//enable vertex array state
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);




				//glBindTexture(GL_TEXTURE_2D, mySprite.uiTextureID);
				glBindTexture(GL_TEXTURE_2D, spriteList[spriteID]->uiTextureID);

				//glBindBuffer(GL_ARRAY_BUFFER, mySprite.uiVBO);
				glBindBuffer(GL_ARRAY_BUFFER, spriteList[spriteID]->uiVBO);

				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
				//now we have UVs to worry about, we need to send that info to the graphics card too
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 8));

				// Enable blending
				glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glDrawArrays(GL_QUADS, 0, sizeof(Vertex));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			/**
			This should be called prior exiting program
			*/
			void Shutdown()
			{
				//glDeleteBuffers(1, &mySprite.uiVBO);
				for (Sprite* s : spriteList)
				{
					glDeleteBuffers(1, &s->uiVBO);
					delete s;
				}
				spriteList.clear();

				glfwTerminate();
			}

			GLuint fontsSpriteSheet;
			Font myFont;
			int fontSheetWidth;
			int fontSheetHeight;
			int fontSheetBPP;
			//Charset charSetDesc;
			uint size = 256;
			Sprite fontChars[256];
			int fontWidth;
			int fontHeight;
		private:
			GLFWwindow* windowHandle;
			GLuint shaderProgram;
			GLuint IDTexture;
			float* orthographicProjection;
			vec4 backgroundColor;



			//Sprite mySprite;
			std::vector<Sprite*> spriteList;

			void CreateShaderProgram()
			{
				shaderProgram = CreateProgram(".\\source\\VertexShader.glsl", ".\\source\\TexturedFragmentShader.glsl");
				//shaderProgram = CreateProgram(".\\source\\VertexShader.glsl", ".\\source\\FlatFragmentShader.glsl");
			}

			GLuint CreateShader(GLenum a_ShaderType, const char* a_ShaderFile)
			{
				std::string shaderCode;
				//open shader file
				std::ifstream shaderStream(a_ShaderFile);
				//if that worked ok, load file line by line
				if (shaderStream.is_open())
				{
					std::string line = "";
					while (std::getline(shaderStream, line))
					{
						shaderCode += "\n" + line;
					}
					shaderStream.close();
				}

				//convert to cstring
				char const* shaderSourcePointer = shaderCode.c_str();

				//create shader ID
				GLuint shader = glCreateShader(a_ShaderType);
				//load source code
				glShaderSource(shader, 1, &shaderSourcePointer, NULL);

				//compile shader
				glCompileShader(shader);

				//check for errors
				GLint status;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
				if (status == GL_FALSE)
				{
					GLint infoLogLength;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

					GLchar* infoLog = new GLchar[infoLogLength + 1];
					glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

					const char* shaderType = NULL;
					switch (a_ShaderType)
					{
					case GL_VERTEX_SHADER:
						shaderType = "vertex";
						break;
					case GL_FRAGMENT_SHADER:
						shaderType = "fragment";
						break;
					}

					fprintf(stderr, "Compile failure in %s shader:\n%s\n", shaderType, infoLog);
					delete[] infoLog;
				}

				return shader;

			}

			GLuint CreateProgram(const char* a_vertex, const char* a_frag)
			{

				std::vector<GLuint> shaderList;

				shaderList.push_back(CreateShader(GL_VERTEX_SHADER, a_vertex));
				shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, a_frag));

				//create shader program ID
				GLuint program = glCreateProgram();

				//attach shaders
				for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
				{
					glAttachShader(program, *shader);
				}

				//link program
				glLinkProgram(program);

				//check for link errors and output them
				GLint status;
				glGetProgramiv(program, GL_LINK_STATUS, &status);
				if (status == GL_FALSE)
				{
					GLint infoLogLength;
					glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

					GLchar* infoLog = new GLchar[infoLogLength + 1];
					glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);

					fprintf(stderr, "Linker failure: %s\n", infoLog);
					delete[] infoLog;
				}

				for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
				{
					glDetachShader(program, *shader);
					glDeleteShader(*shader);
				}
				return program;
			}

			float* getOrtho(float left, float right, float bottom, float top, float a_fNear, float a_fFar)
			{
				//to correspond with mat4 in the shader
				//ideally this function would be part of your matrix class
				//however I wasn't willing to write your matrix class for you just to show you this
				//so here we are in array format!
				//add this to your matrix class as a challenge if you like!
				float* toReturn = new float[12];
				toReturn[0] = 2.0 / (right - left);;
				toReturn[1] = toReturn[2] = toReturn[3] = toReturn[4] = 0;
				toReturn[5] = 2.0 / (top - bottom);
				toReturn[6] = toReturn[7] = toReturn[8] = toReturn[9] = 0;
				toReturn[10] = 2.0 / (a_fFar - a_fNear);
				toReturn[11] = 0;
				toReturn[12] = -1 * ((right + left) / (right - left));
				toReturn[13] = -1 * ((top + bottom) / (top - bottom));
				toReturn[14] = -1 * ((a_fFar + a_fNear) / (a_fFar - a_fNear));
				toReturn[15] = 1;
				return toReturn;
			}

			unsigned int loadTexture(const char* a_pFilename, int & a_iWidth, int & a_iHeight, int & a_iBPP)
			{
				unsigned int uiTextureID = 0;
				//check file exists
				if (a_pFilename != nullptr)
				{
					////read in image data from file
					//unsigned char* pImageData = SOIL_load_image(a_pFilename, &a_iWidth, &a_iHeight, &a_iBPP, SOIL_LOAD_AUTO);

					////check for successful read
					//if (pImageData)
					//{
					//	//create opengl texture handle
					//	uiTextureID = SOIL_create_OGL_texture(pImageData, a_iWidth, a_iHeight, a_iBPP,
					//		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
					//	//clear what was read in from file now that it is stored in the handle
					//	SOIL_free_image_data(pImageData);
					//}

					uiTextureID = SOIL_load_OGL_texture(a_pFilename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);

					//check for errors
					if (uiTextureID == 0)
					{
						std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
					}
					return uiTextureID;
				}
			}

			void UpdateVBO(GLuint& VBO, Vertex* verticeBuffer, int size)
			{
				//bind vbo
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				//allocate space for vertices on the graphics card
				//size of buffer needs to be 3 vec4 for vertices and 3 vec4 for 
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* size, verticeBuffer, GL_STATIC_DRAW);

				//unmap and unbind buffer
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			//void ParseFont(const char* fileName)
			//{
			//	using namespace pugi;
			//	xml_document doc;
			//	//xml_parse_result result = doc.load_file(".\\resources\\fonts\\arial.fnt");
			//	xml_parse_result result = doc.load_file(fileName);
			//	if (!result)
			//	{
			//		std::cout << "Error loading font file, verify path (do the slashes need escaping?)\n";
			//		std::cout << result.description() << "\n";
			//		return;
			//	}

			//	xml_node common = doc.child("font").child("common");

			//	//load the charset common attributes
			//	charSetDesc.lineHeight = std::atoi(common.attribute("lineHeight").value());
			//	charSetDesc.base = std::atoi(common.attribute("base").value());
			//	charSetDesc.scaleW = std::atoi(common.attribute("scaleW").value());
			//	charSetDesc.scaleH = std::atoi(common.attribute("scaleH").value());
			//	charSetDesc.pages = std::atoi(common.attribute("pages").value());


			//	//load each char
			//	for (xml_node Char : doc.child("font").child("chars").children("char"))
			//	{
			//		ushort id = std::atoi(Char.attribute("id").value());
			//		charSetDesc.Chars[id].x = std::atoi(Char.attribute("x").value());
			//		charSetDesc.Chars[id].y = std::atoi(Char.attribute("y").value());
			//		charSetDesc.Chars[id].width = std::atoi(Char.attribute("width").value());
			//		charSetDesc.Chars[id].height = std::atoi(Char.attribute("height").value());
			//		charSetDesc.Chars[id].xOffset = std::atof(Char.attribute("xoffset").value());
			//		charSetDesc.Chars[id].yOffset = std::atof(Char.attribute("yoffset").value());
			//		charSetDesc.Chars[id].xAdvance = std::atof(Char.attribute("xadvance").value());
			//		charSetDesc.Chars[id].page = std::atoi(Char.attribute("page").value());
			//	}

			//}

			//void LoadFontChars()
			//{

			//	/*
			//				Sprite* newSprite = new Sprite;
			//	//glGenBuffers(1, &mySprite.uiVBO);
			//	glGenBuffers(1, &newSprite->uiVBO);

			//	//mySprite.Initialize(shaderProgram, a_width, a_height);
			//	newSprite->Initialize(shaderProgram, a_width, a_height);

			//	int textureWidth = 50;
			//	int textureHeight = 50;
			//	int textureBPP = 4;

			//	//mySprite.uiTextureID = loadTexture(a_fileName, textureWidth, textureHeight, textureBPP);
			//	newSprite->uiTextureID = loadTexture(a_fileName, textureWidth, textureHeight, textureBPP);
			//	newSprite->SetUVCoordinates(UVCoordinates);
			//	spriteList.push_back(newSprite);

			//	//return the sprites index for accessing later without search
			//	return spriteList.size() - 1;

			//	//top left origin?
			//	float left = ch.x;
			//	float top = sheetHeight - (ch.y);//invert origin?
			//	float right = left + ch.width;
			//	float bottom = sheetHeight - (ch.y + ch.height);//invert origin

			//	//normalize
			//	vec4 charUVs = vec4(
			//	left / sheetWidth,
			//	top / sheetHeight,
			//	right / sheetWidth,
			//	bottom / sheetHeight);
			//	textCharID = CreateSprite(".\\resources\\fonts\\arial_0.png", ch.width, ch.height, charUVs);
			//	glm::vec4 position = vec4(MNF::Globals::SCREEN_WIDTH * .5, MNF::Globals::SCREEN_HEIGHT * .5, 0, 1);
			//	MoveSprite(textCharID, position);

			//	*/
			//	for (int i = 0; i < 256; i++)
			//	{
			//		CharDescriptor ch = charSetDesc.Chars[i];
			//		if (ch.height > 0)
			//		{
			//			Sprite fontChar;
			//			glGenBuffers(1, &fontChar.uiVBO);

			//			fontChar.Initialize(shaderProgram, fontWidth, fontHeight);
			//			fontChar.uiTextureID = fontsSpriteSheet;

			//			//top left origin!
			//			float left = ch.x;
			//			float top = fontSheetHeight - ch.y;//flip the y origin
			//			float right = ch.x + ch.width;
			//			float bottom = fontSheetHeight - (ch.y + ch.height);//flip the y origin

			//			//normalize the UVs
			//			glm::vec4 UVs(
			//				left / fontSheetWidth,
			//				top / fontSheetHeight,
			//				right / fontSheetWidth,
			//				bottom / fontSheetHeight);

			//			fontChar.SetUVCoordinates(UVs);
			//			fontChars[i] = fontChar;
			//		}

			//	}

			//	
			//}
		};
	}

#endif