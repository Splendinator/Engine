#pragma once

#include <iostream>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "IO.h"
#include "Shader.h"
#include "Mesh.h"
#include "Object.h"
#include "Matrix4.h"
#include <vector>

class Rasteriser
{
private:


	GLuint program;
	GLint locModel, locView, locProj;

	Matrix4 projection;
	Matrix4 view;

	std::vector<Object *> objects;	//Vector of all the objects will be drawn


	 
	



public:

	const static int SCREEN_WIDTH = 600;
	const static int SCREEN_HEIGHT = 600;

	Rasteriser();

	void update();

	~Rasteriser();

	void addObject(Object *o);
	void removeObject(Object *o);

	void setProjection(const Matrix4 &m);	
	
	void setShader(const Shader &s);
	void bindCamera(Matrix4 &m);



};

inline void Rasteriser::bindCamera(Matrix4 &m)
{
	view = m;
	glUniformMatrix4fv(locView, 1, false, (GLfloat *)&view);
}

inline void Rasteriser::addObject(Object * o)
{
	objects.push_back(o);
}

inline void Rasteriser::removeObject(Object * o)
{
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		if (*it == o) {
			objects.erase(it);
			break;
		}
	}
}

inline void Rasteriser::setProjection(const Matrix4 & m)
{
	projection = m;
	glUniformMatrix4fv(locProj, 1, false, (GLfloat *)&projection);
	
}

inline void Rasteriser::setShader(const Shader & s)
{
	program = s.programID;
	glUseProgram(program);

	//Get locations of MVP;
	locModel = glGetUniformLocation(program, "model");
	locView = glGetUniformLocation(program, "view");
	locProj = glGetUniformLocation(program, "proj");

	std::cout << locModel << " " << locView << " " << locProj;

}