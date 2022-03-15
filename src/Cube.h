#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Cube
{
public:
	struct Vertex {
		glm::vec3 pos;		// Position
		glm::vec3 norm;		// Normal
	};

private:
	unsigned int vao;
	unsigned int vbuf;
	int vcount;

	std::vector<Vertex> vertices;

	glm::mat4 modelMat = glm::mat4(1.0f);

public:
	Cube();
	~Cube();

	void loadMesh(std::string filename);
	void draw();

	glm::mat4 getModelMat()
	{
		return modelMat;
	}
};