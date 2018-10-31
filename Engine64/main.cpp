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



//Moving mouse callback
void handleMouse(int x, int y) {
	static int lastx = x;	//Static so retain value between func calls.
	static int lasty = y;

	o.transform *= Matrix4::translate(float(lastx-x)/1, -float(lasty-y)/1, 0);

	lastx = x;
	lasty = y;

}



//GAME LOOP
void gameLoop(void) {

	o.transform *= Matrix4::rotation(0.1f, 0.1f, 0.1f);

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

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glEnd();

	glutSwapBuffers();


}

void nothing() {};
void nothing(int a, int b) {};

int main(int argc, char** argv) {

	
	//SETUP
	initGL(argc, argv);

	o.transform *= Matrix4::translate(-0.5, 0, 0) * Matrix4::scale(5, 10, 10) * Matrix4::rotation(0, 1, 0);
	o2.transform *= Matrix4::translate(0.5, 0, 0) * Matrix4::rotation(0, 1, 0) * Matrix4::scale(0.5, 1, 1);
	
	r.addObject(&o);
	
	s = Shader("Shaders\\basic.vert", "Shaders\\basic.frag");
	
	r.setShader(s);
	
	//r.setProjection(Matrix4::identity());
	r.bindCamera(Matrix4::identity());

	r.setProjection(Matrix4::Perspective(-1,4,40,1));
	r.bindCamera(Matrix4::translate(0,1.0f,-30.0f));

	glutDisplayFunc(gameLoop);
	glutIdleFunc(gameLoop);

	glutMainLoop();
	

	return 0;

}



