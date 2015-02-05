#include "Sprite.h"

Sprite::Sprite()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);

	float vertices[] =
	{	//position color      UV
		-.5, .5, 1, 0, 0, 0, 0, 0,//top-left
		.5, .5, 0, 1, 0, 1, 0,//top-right
		.5, -.5, 0, 0, 1, 1, 1,//bottom-right
		-.5, -.5, 1, 1, 1, 0, 1//bottom-left
	};

	glBindBuffer(1, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_EBO);

	GLuint elements[] =
	{
		0, 1, 2,
		2, 3, 0
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

Sprite::~Sprite()
{

}