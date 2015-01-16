#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "GL\glew.h"
#include "GL\wglew.h"

//glew must be included first
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "SOIL\SOIL.h"
#include "Globals.h"
//#include "Player.h"
#include "Sprite.h"
#include <fstream>
#include <vector>
#include <string>

typedef glm::vec4 vec4;

namespace GLF
{
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

			IDTexture = glGetUniformLocation(uiProgramTextured, "MVP");
			orthographicProjection = getOrtho(0, MNF::Globals::SCREEN_WIDTH, 0, MNF::Globals::SCREEN_HEIGHT, 0, 100);
			backgroundColor = a_backgroundColor;

			//player.Initialize(vec4(MNF::Globals::SCREEN_WIDTH * .5, MNF::Globals::SCREEN_HEIGHT * .5, 0, 1), vec4(1, 1, 1, 1), uiProgramTextured);
			//mySprite.Initialize(".\\resources\\images\\lobo.png",vec4(MNF::Globals::SCREEN_WIDTH * .5, MNF::Globals::SCREEN_HEIGHT * .5, 0, 1), vec4(1, 1, 1, 1), uiProgramTextured);
			return -1;

		}

		void CreateSprite(const char* a_fileName, int a_width, int a_height)
		{
			mySprite.Initialize(a_fileName, uiProgramTextured, a_width, a_height);
		}

		void MoveSprite(const vec4& a_position)
		{
			mySprite.SetPosition(a_position);
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

		void ClearScreen()
		{
			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void DrawSprite()
		{
			//player.Draw(IDTexture, orthographicProjection);
			mySprite.Draw(IDTexture, orthographicProjection);
		}

		GLuint GetShaderProgram()
		{
			return uiProgramTextured;
		}

		GLuint GetShaderMVP()
		{
			return IDTexture;
		}

		float* GetOrtho()
		{
			return orthographicProjection;
		}

		/**
		This should be called prior exiting program
		*/
		void Shutdown()
		{
			glfwTerminate();
		}

	private:
		GLFWwindow* windowHandle;
		GLuint uiProgramTextured;
		GLuint IDTexture;
		float* orthographicProjection;
		vec4 backgroundColor;

		//Player player;
		Sprite mySprite;

		void CreateShaderProgram()
		{
			uiProgramTextured = CreateProgram(".\\source\\VertexShader.glsl", ".\\source\\TexturedFragmentShader.glsl");
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

	};
}

#endif