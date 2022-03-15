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

	glm::vec3 lookDir;
	glm::vec3 camPos;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 velocity;
	bool cameraMoving;

	Cube* cube;

	int lastMouseX;
	int lastMouseY;

public:
	Renderer();
	~Renderer();

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