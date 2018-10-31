#include "Rasteriser.h"




Rasteriser::Rasteriser()
{

}

//Print out freeglut errors, and pause program.
void Rasteriser::errorHandle(const char *text, va_list val) {
	std::cout << "Error: " << text;
}

//Print out freeglut warnings
void Rasteriser::warningHandle(const char *text, va_list val) {
	std::cout << "Warning: " << text;
}


void Rasteriser::update()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		(**it).draw();
	}

	glutSwapBuffers();


}









//Initialise OpenGL
void Rasteriser::init(int argc, char **argv) {
	
	//Initialise freeGLUT (Window Manager)
	glutInit(&argc, argv);
	glutInitWindowPosition(600, 200);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitErrorFunc(&Rasteriser::errorHandle);
	glutInitWarningFunc(&Rasteriser::warningHandle);
	glutCreateWindow("OpenGL Program");

	//Initialise GLEW (Lets OpenGL work on multiple OS)
	if (glewInit() != GLEW_OK) {
		std::cout << "Error initialising GLEW";
		exit(1);
	}
	
	//======= USER INIT ==========
	glClearColor(0.1, 0.1, 0.1, 1);


	

	

	//============================


	//glutPassiveMotionFunc(&handleMouse);
	//glutMotionFunc(&handleMouse);
	//glutDisplayFunc(&displayCallBack);
	//glutIdleFunc(&displayCallBack);

	//glutMainLoop();


}

Rasteriser::~Rasteriser()
{
	
}





