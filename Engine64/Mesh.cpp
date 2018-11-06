#include "Mesh.h"



Mesh::Mesh()
{
	verts = nullptr;
	tex = nullptr;
}


Mesh::~Mesh()
{
	delete [] verts;
	delete [] tex;
}

void Mesh::buffer(){
	
	//VAO
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);


	//Verts
	glGenBuffers(1, &vertId);
	glBindBuffer(GL_ARRAY_BUFFER, vertId);
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(VERTEX_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_ID);


	//Tex
	glGenBuffers(1, &texId);
	glBindBuffer(GL_ARRAY_BUFFER, texId);
	glBufferData(GL_ARRAY_BUFFER, num * 2 * sizeof(GLfloat), tex, GL_STATIC_DRAW);

	glVertexAttribPointer(TEXTURE_ID, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(TEXTURE_ID);


}

Mesh * Mesh::Triangle() {
	Mesh *m = new Mesh;
	m->num = 3;
	m->verts = new GLfloat[9]{ -1,-1,0,0,1,0,1,-1,0 };
	m->tex = new GLfloat[6]{ 0,0,0.5,1,1,0 };
	return m;
}

Mesh * Mesh::Quad()
{
	Mesh *m = new Mesh;
	m->num = 6;
	m->verts = new GLfloat[18]{ -1,-1,0,-1,1,0,1,-1,0 , -1 ,1 ,0 , 1 ,1,0, 1,-1,0  };
	m->tex = new GLfloat[12]{ 0,0,0,1,1,0,0,1,1,1,1,0 };
	return m;
}
 
