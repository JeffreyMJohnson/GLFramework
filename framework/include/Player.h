#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Entity.h"

class Player : 
	public Entity
{
public:
	Player()
	{
		LoadModelVertices();
		LoadModelUVs();
		verticesBuffer = new Vertex[modelVertices.size()];
		textureWidth = 50;
		textureHeight = 50;
		textureBPP = 4;
		//uiTextureID = loadTexture(".\\resources\\images\\lobo.png", textureWidth, textureHeight, textureBPP);

	}

	void Initialize(glm::vec4& a_position, glm::vec4& a_color, GLuint a_ShaderProgram)
	{
		position = a_position;
		color = a_color;
		glGenBuffers(1, &uiVBO);
		glGenBuffers(1, &uiIBO);
		programShader = a_ShaderProgram;
		UpdateVertices();
		uiTextureID = loadTexture(".\\resources\\images\\lobo.png", textureWidth, textureHeight, textureBPP);
	}

	~Player()
	{
		delete verticesBuffer;
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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
		//now we have UVs to worry about, we need to send that info to the graphics card too
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 8));

		glDrawElements(GL_QUADS, modelVertices.size(), GL_UNSIGNED_BYTE, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	void LoadModelVertices()
	{
		int width = 500;
		int height = 500;
		float hWidth = width * .5;
		float hHeight = height * .5;
		float posX = 1024 * .5;
		float posY = 768 * .5;

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