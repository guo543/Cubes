#pragma once
#include "Cube.h"

class Renderer
{
public:
	enum Direction
	{
		FORWARD, BACKWARD, LEFT, RIGHT
	};
private:
	int width = 1000;
	int height = 1000;

	int shader;
	unsigned int xformLoc;
	unsigned int highlightedLoc;
	unsigned int texUnitLoc;

	glm::vec3 lookDir;
	glm::vec3 camPos;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 velocity;
	bool cameraMoving;

	std::vector<Cube*> cubes;

	int lastMouseX;
	int lastMouseY;

public:
	Renderer();
	~Renderer();

	void readConfig(std::string filename);

	void render();
	void updateProjView();
	void moveCamera();
	void addVelocity(Direction dir);
	void zeroVelocity()
	{
		velocity = glm::vec3(0.0f);
	}
	void rotateCamera(int x, int y);
	void cameraMoveSwitch();
	void resize(int width, int height);

	bool isCameraMoving()
	{
		return cameraMoving;
	}
};