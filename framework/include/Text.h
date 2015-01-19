#include <vector>
#include <iostream>

//struct Vertex
//{
//	float fPositions[4];
//	float fColors[4];
//	float fUVs[2];
//};

class Text
{
public:
	void Initialize(GLuint& a_ShaderProgram, int width, int height)
	{

		//this->width = width;
		//this->height = height;
		//this->fileName = fileName;
		//LoadModelVertices();
		//verticesBuffer = new Vertex[modelVertices.size()];
		//position = glm::vec4();
		//UVCoordinates = glm::vec4(0, 1, 1, 0);
		//LoadModelUVs();
		//UpdateVertices();

	}

	Vertex* verticesBuffer;
	glm::vec4 UVCoordinates;//rectangle minX, maxY, maxX, minY
private:
	std::vector<glm::vec4> modelVertices;
	std::vector<glm::vec2> modelUVs;
	glm::vec4 position;

	int width;
	int height;
	char* fileName;
};