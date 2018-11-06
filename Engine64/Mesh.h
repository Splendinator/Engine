#pragma once
#include "GL/glew.h"
#include <string>
#include <iostream>

enum ID {VERTEX_ID, TEXTURE_ID};

class Mesh
{
private:


public:

	GLfloat *verts;
	GLfloat *tex;

	GLuint vertId;
	GLuint texId;

	GLuint vaoId;

	int num = 0;



	Mesh();
	~Mesh();
	Mesh(std::string filepath);	//Read from file TODO:

	static Mesh *Triangle();
	static Mesh *Quad();

	void buffer(); 
	
	int getLength() { return num; }


	

};

