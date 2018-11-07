#include "Initialize.h"

//Print out freeglut errors, and pause program.
void errorHandle(const char *text, va_list val) {
	std::cout << "Error: " << text;
}

//Print out freeglut warnings
void warningHandle(const char *text, va_list val) {
	std::cout << "Warning: " << text;
}



void Initialize::init(int argc, char** argv) {
	//Initialise freeGLUT (Window Manager)
	glutInit(&argc, argv);
	glutInitWindowPosition(600, 200);
	glutInitWindowSize(Rasteriser::SCREEN_WIDTH, Rasteriser::SCREEN_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
	glutInitErrorFunc(errorHandle);
	glutInitWarningFunc(warningHandle);
	glutCreateWindow("OpenGL Program");

	//Initialise GLEW (Lets OpenGL work on multiple OS)
	if (glewInit() != GLEW_OK) {
		std::cout << "Error initialising GLEW";
		exit(1);
	}

	//glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.1, 0.1, 0.1, 1);

}