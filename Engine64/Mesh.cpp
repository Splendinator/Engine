#include "Mesh.h"



Mesh::Mesh()
{
	verts = new GLfloat[9] {-1,-1,0,0,1,0,1,-1,0 };
	num = 9;
}


Mesh::~Mesh()
{
	delete [] verts;
}

void Mesh::buffer() const{
	GLuint bufferNum;
	glGenBuffers(1, &bufferNum);
	glBindBuffer(GL_ARRAY_BUFFER, bufferNum);
	glBufferData(GL_ARRAY_BUFFER, num * sizeof(GLfloat), verts, GL_STATIC_DRAW);
}
