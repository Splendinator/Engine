#pragma once

#include <iostream>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "IO.h"
#include "Shader.h"
#include "Mesh.h"
#include "Object.h"
#include "Matrix4.h"
#include "Camera.h"
#include <vector>
#include <algorithm>

class Rasteriser
{
private:


	//GLuint program;
	GLint locModel;
	GLint locView;
	GLint locProj;


	Matrix4 projection;
	//Matrix4 orthogonal; 
	
	Camera *camera;

	std::vector<Object *> objects;	//Vector of all the objects will be drawn


	 
	



public:

	const static int SCREEN_WIDTH = 1280;
	const static int SCREEN_HEIGHT = 720;

	Rasteriser();

	void update();

	~Rasteriser();

	void addObject(Object *o);
	void removeObject(Object *o);

	void setProjection(const Matrix4 &m);	

	void bindCamera(Camera *c);



};

inline void Rasteriser::bindCamera(Camera *c)
{
	camera = c;
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
}

