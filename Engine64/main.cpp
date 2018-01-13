#include <iostream>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "IO.h"
#include "Shader.h"


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

	GLuint bufferNum;
	glGenBuffers(1, &bufferNum);
	GLfloat triangle[9] = { -1,-1,0,0,1,0,1,-1,0 };
	glBindBuffer(GL_ARRAY_BUFFER, bufferNum);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &triangle, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	Shader::compileProgram("Shaders\\basic.vert", "Shaders\\basic.frag");

}

//Called by the window once per frame.
void displayCallBack() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);

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
	glutMainLoop();

	return 0;
}

