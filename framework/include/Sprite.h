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
	Sprite()
	{


	}

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
		glGenBuffers(1, &uiVBO);
		programShader = a_ShaderProgram;
		UpdateVertices();

	}

	void SetPosition(const glm::vec4& a_position)
	{
		position = a_position;
		UpdateVertices();
	}

	void CleanUp()
	{
		glDeleteBuffers(1, &uiVBO);
	}

	void Update(GLFWwindow* windowHandle)
	{
		if (glfwGetKey(windowHandle, GLFW_KEY_W) == GLFW_PRESS)
		{
			//move forward 
			//std::cout << "move forward" << std::endl;
			for (int i = 0; i < 3; i++)
			{
				modelVertices[i].y += .01f;
			}
			UpdateVertices();
		}

		if (glfwGetKey(windowHandle, GLFW_KEY_S))
		{
			//move backwards
			for (int i = 0; i < modelVertices.size(); i++)
			{
				modelVertices[i].y -= .01f;
			}
			UpdateVertices();
		}

		if (glfwGetKey(windowHandle, GLFW_KEY_A))
		{
			//move backwards
			for (int i = 0; i < modelVertices.size(); i++)
			{
				modelVertices[i].x -= .01f;
			}
			UpdateVertices();
		}
		if (glfwGetKey(windowHandle, GLFW_KEY_D))
		{
			//move backwards
			for (int i = 0; i < modelVertices.size(); i++)
			{
				modelVertices[i].x += .01f;
			}
			UpdateVertices();
		}
	}

	void Draw(GLuint uniformLocationID, float* orthoProjection)
	{
		glUseProgram(programShader);

		//send ortho projection info to shader
		glUniformMatrix4fv(uniformLocationID, 1, GL_FALSE, orthoProjection);

		//enable vertex array state
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindTexture(GL_TEXTURE_2D, uiTextureID);
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));
		//now we have UVs to worry about, we need to send that info to the graphics card too
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));

		glDrawArrays(GL_QUADS, 0, sizeof(Vertex));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	GLuint uiTextureID;
private:
	std::vector<glm::vec4> modelVertices;
	std::vector<glm::vec2> modelUVs;
	glm::vec4 position;
	GLuint uiVBO;
	
	GLuint programShader;
	Vertex* verticesBuffer;
	int width;
	int height;
	char* fileName;

	void UpdateVBO()
	{
		//bind vbo
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		//allocate space for vertices on the graphics card
		//size of buffer needs to be 3 vec4 for vertices and 3 vec4 for 
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * modelVertices.size(), verticesBuffer, GL_STATIC_DRAW);
		//get pointer to allocated space on the graphics card
		//GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		//copy data to graphics card
		//memcpy(vBuffer, verticesBuffer, sizeof(Vertex) * modelVertices.size());
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void UpdateVertices()
	{
		for (int i = 0; i < modelVertices.size(); i++)
		{
			verticesBuffer[i].fPositions[0] = modelVertices[i].x + position.x;
			verticesBuffer[i].fPositions[1] = modelVertices[i].y + position.y;
			verticesBuffer[i].fPositions[2] = 0;
			verticesBuffer[i].fPositions[3] = 1;
			verticesBuffer[i].fColors[0] = 1;
			verticesBuffer[i].fColors[1] = 1;
			verticesBuffer[i].fColors[2] = 1;
			verticesBuffer[i].fColors[3] = 0;
			verticesBuffer[i].fUVs[0] = modelUVs[i].x;
			verticesBuffer[i].fUVs[1] = modelUVs[i].y;
		}
		UpdateVBO();
	}


	void LoadModelVertices()
	{
		float hWidth = width * .5;
		float hHeight = height * .5;

		modelVertices.push_back(glm::vec4(-hWidth, hHeight, 0, 1));
		modelVertices.push_back(glm::vec4(hWidth, hHeight, 0, 1));
		modelVertices.push_back(glm::vec4(hWidth, -hHeight, 0, 1));
		modelVertices.push_back(glm::vec4(-hWidth, -hHeight, 0, 1));
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