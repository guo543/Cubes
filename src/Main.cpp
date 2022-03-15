#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Renderer.h"


int width = 1000;
int height = 1000;
Renderer* renderer;

void display();
void mouseMove(int x, int y);
void keyPress(unsigned char key, int x, int y);
void keyRelease(unsigned char key, int x, int y);
void reshape(int width, int height);
void idle();

void printInstruction()
{
	std::cout << "----------------------------------------\nPress ESC to switch on/off moving mode.\n----------------------------------------";
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(width, height);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	// Create the window
	glutCreateWindow("Cubes");
	if (glewInit() != GLEW_OK) {
		std::cout << "Error" << std::endl;
	}
	//glutFullScreen();

	renderer = new Renderer();

	glutDisplayFunc(display);
	glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	printInstruction();

	glutMainLoop();

	delete renderer;
}

void display()
{
	renderer->render();

	glutSwapBuffers();
	return;
}

void mouseMove(int x, int y) {
	if (renderer->isCameraMoving()) {

		renderer->rotateCamera(x, y);

		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
		glutPostRedisplay();
	}
}

void keyPress(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		renderer->addVelocity(Renderer::FORWARD);
		break;
	case 's':
		renderer->addVelocity(Renderer::BACKWARD);
		break;
	case 'a':
		renderer->addVelocity(Renderer::LEFT);
		break;
	case 'd':
		renderer->addVelocity(Renderer::RIGHT);
		break;
	}
}

void keyRelease(unsigned char key, int x, int y) {
	if (key == 'w' || key == 'a' || key == 's' || key == 'd')
	{
		renderer->zeroVelocity();
	}
	else if (key == 27) {	// Escape key
		renderer->cameraMoveSwitch();
	}
}

void reshape(int width, int height) {
	//std::cout << width << ", " << height << "\n";
	renderer->resize(width, height);
}

void idle()
{
	renderer->moveCamera();
	glutPostRedisplay();
}