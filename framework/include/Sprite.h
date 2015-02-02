#ifndef _SPRITE_H_
#define _SPRITE_H_
#include "GL\glew.h"
#include "GL\wglew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "SOIL\SOIL.h"
#include <vector>
#include <iostream>

struct Vertex
{
	float fPositions[4];
	float fColors[4];
	float fUVs[2];
};

class Sprite
{
public:

	~Sprite()
	{
		delete verticesBuffer;
	}
	void Initialize(GLuint& a_ShaderProgram, int width, int height)
	{

		this->width = width;
		this->height = height;
		this->fileName = fileName;
		LoadModelVertices();
		LoadModelUVs();
		verticesBuffer = new Vertex[modelVertices.size()];
		position = glm::vec4();
		UpdateVertices();

	}

	void SetPosition(const glm::vec4& a_position)
	{
		position = a_position;
		UpdateVertices();
	}

	GLuint uiTextureID;
	GLuint uiVBO;
	GLuint uiVAO;
	GLuint shaderProgram;
	Vertex* verticesBuffer;

	glm::mat4 transform;
	glm::vec3 translation, scale;
	float rotZ;
	glm::vec4 position;

	GLint uniMVP;
	GLuint uniTranslate;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatirx;
	glm::mat4 modelViewProjectionMatrix;

private:
	std::vector<glm::vec4> modelVertices;
	std::vector<glm::vec2> modelUVs;
	
	
	int width;
	int height;
	char* fileName;

	void UpdateVertices()
	{
		//for (int i = 0; i < modelVertices.size(); i++)
		//{
		//	verticesBuffer[i].fPositions[0] = modelVertices[i].x + position.x;
		//	verticesBuffer[i].fPositions[1] = modelVertices[i].y + position.y;
		//	verticesBuffer[i].fPositions[2] = 0;
		//	verticesBuffer[i].fPositions[3] = 1;
		//	verticesBuffer[i].fColors[0] = 1;
		//	verticesBuffer[i].fColors[1] = 1;
		//	verticesBuffer[i].fColors[2] = 1;
		//	verticesBuffer[i].fColors[3] = 0;
		//	verticesBuffer[i].fUVs[0] = modelUVs[i].x;
		//	verticesBuffer[i].fUVs[1] = modelUVs[i].y;
		//}
	}

	void LoadModelVertices()
	{
		float hWidth = width * .5;
		float hHeight = height * .5;
		Vertex v;


		//modelVertices.push_back(glm::vec4(-hWidth, hHeight, 0, 1));
		//modelVertices.push_back(glm::vec4(hWidth, hHeight, 0, 1));
		//modelVertices.push_back(glm::vec4(hWidth, -hHeight, 0, 1));
		//modelVertices.push_back(glm::vec4(-hWidth, -hHeight, 0, 1));

	}

	void LoadModelUVs()
	{
		modelUVs.push_back(glm::vec2(0, 1));
		modelUVs.push_back(glm::vec2(1, 1));
		modelUVs.push_back(glm::vec2(1, 0));
		modelUVs.push_back(glm::vec2(0, 0));
	}
};

#endif