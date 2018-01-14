#include <iostream>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "IO.h"
#include "Shader.h"
#include "Mesh.h"
#include "Object.h"
#include "Matrix4.h"

Mesh m = Mesh();
Object o = Object(&m);
Object o2 = Object(&m);
GLuint p;

//Print out freeglut errors, and pause program.
void errorHandle(const char *text, va_list val) {
	std::cout << "E " << text;
}

//Print out freeglut warnings
void warningHandle(const char *text, va_list val) {
	std::cout << "W " << text;
}


//Initialise OpenGL
void initOpenGL() {
	glClearColor(0.1,0.1,0.1,1);


	p = Shader::compileProgram("Shaders\\basic.vert", "Shaders\\basic.frag");

	o.transform = o.transform * Matrix4::translate(-0.5, 0, 0) * Matrix4::scale(0.5, 0.5, 0.5);
	o2.transform = o2.transform * Matrix4::translate(0.5, 0, 0) * Matrix4::scale(0.5, 0.5, 0.5);

	o.LinkProgram(p);
	o2.LinkProgram(p);

}

//Called by the window once per frame.
void displayCallBack() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	o.Draw();
	o2.Draw();

	glutSwapBuffers();

}

int main(int argc, char** argv) {

	//Initialise freeGLUT (Window Manager)
	glutInit(&argc, argv);
	glutInitWindowPosition(600, 200);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitErrorFunc(&errorHandle);
	glutInitWarningFunc(&warningHandle);
	glutCreateWindow("OpenGL Program");

	//Initialise GLEW (Lets OpenGL work on multiple OS)
	if (glewInit() != GLEW_OK) {
		std::cout << "Error initialising GLEW";
		exit(1);
	}
	
	initOpenGL();
	

	glutDisplayFunc(&displayCallBack);
	glutIdleFunc(&displayCallBack);

	glutMainLoop();

	return 0;
}

