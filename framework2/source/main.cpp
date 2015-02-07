#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"
#include <string>

#include <time.h>

//#include "Sprite.h"

#include "FontManager.h"

#include "Animation.h"
//#include "pugixml\pugixml.hpp"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

GLFWwindow* window;
GLuint shaderProgram;
double totalTime;
double deltaTime;

void Initialize();
void Destroy();
void CreateShaderProgram();
GLuint CreateProgram(const char* a_vertex, const char* a_frag);
GLuint CreateShader(GLenum a_ShaderType, const char* a_ShaderFile);
void HandleUI(Sprite& s, Animation& a);
void ResetDeltaTime();

//debug
int animFrame = 1;
Font font;


int main()
{
	Initialize();
	//CreateShaderProgram();
	Sprite s;
	s.Initialize(25, 25, ".\\resources\\images\\testTexture.png", true);
	s.translation = glm::vec3(SCREEN_WIDTH * .5, SCREEN_HEIGHT * .5, 0);
	s.UpdateTransform();
	//s.SetUV(0, 0, .25, .25);

	//work on font
	FontManager fm;
	fm.Initialize(".\\resources\\fonts\\", "arial.fnt");

	glm::vec2 currentFrame(0,2);
	glm::vec2 sheetSize(50, 50);
	glm::vec2 spriteCount(4, 4);
	glm::vec2 frameSize(sheetSize.x / spriteCount.x, sheetSize.y / spriteCount.y);
	//normalize
	s.SetUV(
		(currentFrame.x * frameSize.x) / sheetSize.x,
		(currentFrame.y * frameSize.y) / sheetSize.y,
		((currentFrame.x * frameSize.x) + frameSize.x) / sheetSize.x,
		((currentFrame.y * frameSize.y) + frameSize.y) / sheetSize.y
		);

	//work on animation
	Animation anim;
	deltaTime = 0;
	totalTime = 0;



	while (!glfwWindowShouldClose(window))
	{
		glClearColor(.5, .5, .5, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		ResetDeltaTime();
		
		
		s.Update();
		s.Draw();

		fm.DrawText("Foo to the Bar...", 200,200);

		fm.DrawText("1234567890", 200, 100);

		anim.Update(deltaTime);
		anim.Draw();

		HandleUI(s, anim);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Destroy();

	return 0; 
}

void ResetDeltaTime()
{
	deltaTime = glfwGetTime();
	totalTime += deltaTime;
	glfwSetTime(0);
}

void Initialize()
{
	glfwInit();
	//create the window
	//windowed
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Bitches!", nullptr, nullptr);
	//full screen
	//window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Bitches!", glfwGetPrimaryMonitor(), nullptr);

	//make window active
	glfwMakeContextCurrent(window);

	//force GLEW to use a modern OpenGL method for checking if function is available
	glewExperimental = GL_TRUE;
	//initialize GLEW
	glewInit();

	totalTime = 0.0;
	deltaTime = 0.0;
}

void Destroy()
{
	glfwTerminate();
}

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

void HandleUI(Sprite& s, Animation& a)
{
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		s.translation += glm::vec3(-.1, 0, 0);
		a.mSprite.translation += glm::vec3(-.1, 0, 0);
		s.UpdateTransform();
		a.mSprite.UpdateTransform();
		a.mIsWalking = true;
		if (a.mDirection == 1)
			a.SwitchDirection();
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		s.translation += glm::vec3(.1, 0, 0);
		a.mSprite.translation += glm::vec3(.1, 0, 0);
		
		s.UpdateTransform();
		a.mSprite.UpdateTransform();
		a.mIsWalking = true;
		if (a.mDirection == -1)
			a.SwitchDirection();
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		s.translation += glm::vec3(0, .1, 0);
		s.UpdateTransform();
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		s.translation += glm::vec3(0, -.1, 0);
		s.UpdateTransform();
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_TAB))
	{
		if (animFrame == 1)
		{
			animFrame = 2;
			s.SetUV(.5, .5, .75, .75);
		}
		else
		{
			animFrame = 1;
			s.SetUV(.25, .25, .5, .5);
		}
	}
	a.mIsWalking = false;
}