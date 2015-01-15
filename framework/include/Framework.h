#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "GL\glew.h"
#include "GL\wglew.h"

//glew must be included first
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
//#include "Sprite.h"
#include "Player.h"
#include "Globals.h"


typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef MNF::Globals MN;

namespace GLF
{

	
	class Framework
	{
	public:
		//Sprite mySprite;
		/**
		Initialize GLFramework
		params:
			a_screenWidth - width of the window to create
			a_screenHeight - height of the window to create
			a_title - title of the window to create.
		returns:
			returns -1 if Initialize has no errors else returns 0
		*/

		Player playerInstance;

		int Initialize(int a_screenWidth, int a_screenHeight, const char* a_title)
		{
			screenWidth = a_screenWidth;
			screenHeight = a_screenHeight;

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

			uiProgramTextured = CreateProgram(".\\source\\VertexShader.glsl", ".\\source\\TexturedFragmentShader.glsl");
			matrixVariablePosition = glGetUniformLocation(uiProgramTextured, "MVP");
			orthographicProjection = getOrtho(0, screenWidth, 0, screenHeight, 0, 100);
			//glGenBuffers(1, &uiVBO);


			playerInstance.Initialize(vec4(screenWidth * .5, screenHeight * .5, 0, 1), vec4(1, 1, 1, 1), uiProgramTextured);

			//mySprite.UpdateVertices();

			return -1;

		}

		/**
		Set the background color to given color. Values should be from 0-1.
		*/
		void SetBackgroundColor(const vec4 color)
		{
			if (color.r < 0 || color.r > 1 ||
				color.g < 0 || color.g > 1 ||
				color.b < 0 || color.b > 1 ||
				color.a < 0 || color.a > 1)
				return;
			clearColor = color;
		}

		/**
		This function to be called each frame to update current frame
		returns true if update completes with no error else returns false
		*/
		bool FrameworkUpdate()
		{
			if (glfwWindowShouldClose(windowHandle))
				return false;
			//playerInstance.Update(windowHandle);
			//playerInstance.Draw(matrixVariablePosition, orthographicProjection);

			glfwSwapBuffers(windowHandle);
			glfwPollEvents();

			return true;
		}

		void ClearScreen()
		{
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		//void DrawSprite()
		//{
		//	UpdateVBO();

		//	glUseProgram(uiProgramTextured);
		//	glUniformMatrix4fv(matrixVariablePosition, 1, GL_FALSE, orthographicProjection);

		//	glEnableVertexAttribArray(0);
		//	glEnableVertexAttribArray(1);
		//	glEnableVertexAttribArray(2);

		//	glBindTexture(GL_TEXTURE_2D, mySprite.GetTextureID());
		//	glBindBuffer(GL_ARRAY_BUFFER, uiVBO);

		//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 3));
		//	//now we have UVs to worry about, we need to send that info to the graphics card too
		//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 7));

		//	glDrawArrays(GL_QUADS, 0, 4);
		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		//	glBindTexture(GL_TEXTURE_2D, 0);
		//}

		/**
		This should be called prior exiting program
		*/
		void Shutdown()
		{
			glfwTerminate();
		}


	protected:

	private:
		GLFWwindow* windowHandle;
		int screenWidth;
		int screenHeight;
		vec4 clearColor;//background color
		GLuint uiProgramTextured; //compiled shader program
		GLuint matrixVariablePosition; //var location in shader
		float* orthographicProjection;
		GLuint uiVBO;



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

		//void UpdateVBO()
		//{
		//	//bind vbo
		//	glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		//	//allocate space for vertices on the graphics card
		//	//size of buffer needs to be 3 vec4 for vertices and 3 vec4 for 
		//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 4, NULL, GL_STATIC_DRAW);
		//	//get pointer to allocated space on the graphics card
		//	GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		//	//copy data to graphics card
		//	memcpy(vBuffer, mySprite.GetModelVertices(), sizeof(Vertex)* 4);
		//	//unmap and unbind buffer
		//	glUnmapBuffer(GL_ARRAY_BUFFER);
		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		//}
	};
}

#endif