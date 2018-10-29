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
public:
	Rasteriser();

	void update();
	void init(int argc, char **argv);

	~Rasteriser();

	void addObject(Object *o);
	void removeObject(Object *o);

private:
	GLuint program;

	static void errorHandle(const char *text, va_list val);
	static void warningHandle(const char *text, va_list val);

	

	std::vector<Object *> objects;	//Vector of all the objects will be drawn


};

