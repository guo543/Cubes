#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Cube
{
public:
	struct Vertex {
		glm::vec3 pos;		// Position
		glm::vec3 norm;	    // Normal
		glm::vec2 uv;       // Texture Coordinates
	};

private:
	unsigned int vao;
	unsigned int vbuf;
	int vcount;
	unsigned int texture;

	std::vector<Vertex> vertices;

	// Bounding box
	glm::vec3 minBB;
	glm::vec3 maxBB;

	glm::mat4 modelMat = glm::mat4(1.0f);

public:
	Cube();
	~Cube();

	void loadMesh(std::string filename);
	void loadTexture(std::string filename);
	void activateTexture();
	void draw();

	glm::vec3 getMinBB()
	{
		return minBB;
	}
	glm::vec3 getMaxBB()
	{
		return maxBB;
	}
	glm::mat4 getModelMat()
	{
		return modelMat;
	}
	void setMinBB(glm::vec3 minBB)
	{
		this->minBB = minBB;
	}
	void setMaxBB(glm::vec3 maxBB)
	{
		this->maxBB = maxBB;
	}
	void setModelMat(glm::mat4 modelMat)
	{
		this->modelMat = modelMat;
	}
};