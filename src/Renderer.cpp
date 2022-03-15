#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"
#include "Util.h"

Renderer::Renderer()
{
	//std::cout << "Renderer Created" << std::endl;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_NONE);

	shader = CreateShader("shaders/v.glsl", "shaders/f.glsl");
	xformLoc = glGetUniformLocation(shader, "xform");

	camPos = glm::vec3(0.0f, 2.0f, 10.0f);
	lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	updateProjView();
	cube = new Cube();

	lastMouseX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	lastMouseY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	cameraMoving = true;
	velocity = glm::vec3(0.0f);
}

Renderer::~Renderer()
{
	if (shader)	glDeleteProgram(shader);
	delete cube;
}

void Renderer::render()
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set shader to draw with
	glUseProgram(shader);

	glm::mat4 xform(1.0f);

	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -10.0f));
	xform = proj * view * cube->getModelMat();

	glUniformMatrix4fv(xformLoc, 1, GL_FALSE, glm::value_ptr(xform));

	cube->draw();

	glUseProgram(0);
}

void Renderer::updateProjView()
{
	view = glm::lookAt(camPos, camPos + lookDir, glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void Renderer::moveCamera()
{
	camPos[0] += velocity[0];
	camPos[2] += velocity[2];

	updateProjView();
}

void Renderer::addVelocity(Direction dir)
{
	//velocity[0] = (forward ? lookDir[0] : -lookDir[0]) * 0.05;
	//velocity[2] = (forward ? lookDir[2] : -lookDir[2]) * 0.05;
	glm::vec3 tmpLookDir = glm::vec3(lookDir[0], 0.0f, lookDir[2]);
	glm::vec3 tmp = glm::vec3(0.0f, 1.0f, 0.0f);
	switch (dir)
	{
	case FORWARD:
		velocity[0] += lookDir[0];
		velocity[2] += lookDir[2];
		break;
	case BACKWARD:
		velocity[0] -= lookDir[0];
		velocity[2] -= lookDir[2];
		break;
	case LEFT:
		tmpLookDir = glm::normalize(glm::cross(tmp, tmpLookDir));
		velocity[0] += tmpLookDir[0];
		velocity[2] += tmpLookDir[2];
		break;
	case RIGHT:
		tmpLookDir = glm::normalize(glm::cross(tmpLookDir, tmp));
		velocity[0] += tmpLookDir[0];
		velocity[2] += tmpLookDir[2];
		break;
	}
	velocity = glm::normalize(velocity) * 0.05f;
	updateProjView();
}

void Renderer::rotateCamera(int x, int y)
{
	int deltaX = x - width / 2;
	int deltaY = y - height / 2;
	int newX = lastMouseX + deltaX;
	int newY = lastMouseY + deltaY;

	//std::cout << deltaX << ", " << deltaY << "\n";
	lastMouseX = newX;
	lastMouseY = newY;
	deltaX = width / 2 - newX;
	deltaY = height / 2 - newY;
	glm::vec3 tmp = glm::vec3(0.0f, 0.0f, -1.0f);
	float rotateX = deltaY * 0.08f;
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotateX), glm::vec3(1.0f, 0.0f, 0.0f));
	lookDir = glm::normalize(rotX * glm::vec4(tmp, 0.0f));

	float rotateY = deltaX * 0.08f;
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	lookDir = glm::normalize(rotY * glm::vec4(lookDir, 0.0f));
	updateProjView();
}

void Renderer::cameraMoveSwitch()
{
	cameraMoving = !cameraMoving;
	if (cameraMoving) {
		glutWarpPointer(width / 2, height / 2);
		glutSetCursor(GLUT_CURSOR_NONE);
	} else {
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
}

void Renderer::resize(int width, int height)
{
	this->width = width;
	this->height = height;
	glViewport(0, 0, width, height);
	updateProjView();
}