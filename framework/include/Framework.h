#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "GL\glew.h"
#include "GL\wglew.h"

//glew must be included first
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "SOIL\SOIL.h"
#include "Globals.h"
#include "Sprite.h"
#include <fstream>
#include <vector>
#include <string>

typedef glm::vec4 vec4;
typedef unsigned int uint;

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

			return -1;

		}

		/**
		Create a 2D sprite from given filename with given width and height.
		returns a unique Sprite ID
		*/
		const uint CreateSprite(char* a_fileName, int a_width, int a_height)
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

		void MoveSprite(const uint spriteID, const vec4& a_position)
		{
			Sprite* s = spriteList[spriteID];
			//mySprite.SetPosition(a_position);
			s->SetPosition(a_position);

			//UpdateVBO(mySprite.uiVBO, mySprite.verticesBuffer, 4);
			UpdateVBO(s->uiVBO, s->verticesBuffer, 4);
		}

		bool FrameworkUpdated()
		{
			if (glfwWindowShouldClose(windowHandle))
			{
				return false;
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
			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
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
			glBindTexture(GL_TEXTURE_2D,spriteList[spriteID]->uiTextureID);

			//glBindBuffer(GL_ARRAY_BUFFER, mySprite.uiVBO);
			glBindBuffer(GL_ARRAY_BUFFER, spriteList[spriteID]->uiVBO);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));
			//now we have UVs to worry about, we need to send that info to the graphics card too
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));

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
			//ofstream file;
			//file.open(a_vertex, ios::in);
			//cout << file.is_open() << endl;

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

		void UpdateVBO(GLuint& VBO, Vertex* verticeBuffer, int size )
		{
			//bind vbo
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//allocate space for vertices on the graphics card
			//size of buffer needs to be 3 vec4 for vertices and 3 vec4 for 
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, verticeBuffer , GL_STATIC_DRAW);

			//unmap and unbind buffer
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	};
}

#endif