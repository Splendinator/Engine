#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include <string.h>
#include "Mesh.h"
#include "Object.h"
#include "Rasteriser.h"



Rasteriser r;

Mesh *m = Mesh::Triangle();

Object o = Object(m);
Object o2 = Object(m);

Shader s;

Camera cam;


//Moving mouse callback
void handleMouse(int x, int y) {
	static int lastx = x;	//Static so retain value between func calls.
	static int lasty = y;

	cam.rollYaw((x - lastx) / 100.f);
	cam.rollPitch((y - lasty) / 100.f);

	lastx = x;
	lasty = y;
}



//GAME LOOP
void gameLoop(void) {

	//o.transform *= Matrix4::rotation(0.01f, 0.01f, 0.01f);
	//c.move(0.01f, 0, 0);
	r.update();
	
}


//Print out freeglut errors, and pause program.
void errorHandle(const char *text, va_list val) {
	std::cout << "Error: " << text;
}

//Print out freeglut warnings
void warningHandle(const char *text, va_list val) {
	std::cout << "Warning: " << text;
}


void initGL(int argc, char** argv) {
	//Initialise freeGLUT (Window Manager)
	glutInit(&argc, argv);
	glutInitWindowPosition(600, 200);
	glutInitWindowSize(Rasteriser::SCREEN_WIDTH, Rasteriser::SCREEN_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitErrorFunc(errorHandle);
	glutInitWarningFunc(warningHandle);
	glutCreateWindow("OpenGL Program");

	//GLUT input settings
	//glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	//Initialise GLEW (Lets OpenGL work on multiple OS)
	if (glewInit() != GLEW_OK) {
		std::cout << "Error initialising GLEW";
		exit(1);
	}

	glClearColor(0.1, 0.1, 0.1, 1);

}

void handleKeyDown(unsigned char c, int x, int y) {
	switch (c) {
	case ('a'): 
		cam.move(-0.1f, 0, 0);
		break;
	case ('d'):
		cam.move(0.1f, 0, 0);
		break;
	case ('w'):
		cam.move(0, 0, -0.1f);
		break;
	case ('s'):
		cam.move(0, 0, 0.1f);
		break;
	case ('z'):
		cam.move(0, -0.1f, 0);
		break;
	case (' '):
		cam.move(0, 0.1f, 0);


		break;
	}
	glutPostRedisplay();
}

void handleKeyUp(unsigned char c, int x, int y) {
	switch (c) {
	case ('a'):
		cam.move(-0.1f, 0, 0);
		break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv) {

	
	//SETUP
	initGL(argc, argv);

	//OBJECTS
	o.transform *= Matrix4::translate(0, 0, -5);
	r.addObject(&o);
	
	//SHADER
	s = Shader("Shaders\\basic.vert", "Shaders\\basic.frag");
	r.setShader(s);
	
	//MVP
	r.bindCamera(&cam);
	r.setProjection(Matrix4::Perspective(1,10,40,1));

	glutDisplayFunc(gameLoop);
	glutIdleFunc(gameLoop);
	glutKeyboardFunc(handleKeyDown);
	
	//glutKeyboardUpFunc(handleKeyUp);
	glutPassiveMotionFunc(handleMouse);

	glutMainLoop();
	

	return 0;

}



