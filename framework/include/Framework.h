#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "GL\glew.h"
#include "GL\wglew.h"

//glew must be included first
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"

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
		static int Initialize(int a_screenWidth, int a_screenHeight, const char* a_title)
		{
			if (!glfwInit())
				return 0;
			GLFWwindow* winHandle = glfwCreateWindow(a_screenWidth, a_screenHeight, a_title, NULL, NULL);
			
			if (winHandle == NULL)
				return 0;
			glfwMakeContextCurrent(winHandle);

			//init glew
			if (glewInit() != GLEW_OK)
			{
				//glew didn't start so shutdown glfw and return error
				glfwTerminate();
				return 0;
			}

			return -1;

		}

		/**
		This should be called prior exiting program
		*/
		static void Shutdown()
		{
			glfwTerminate();
		}

		static int x;
		
	};
	int x;
}

#endif