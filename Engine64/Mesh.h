#pragma once
#include "GL/glew.h"
#include "Vector.h"
#include <string>
#include <iostream>

enum ID {VERTEX_ID, TEXTURE_ID, ALPHA_ID, INDICIES_ID};

class Mesh
{
private:


public:

	//Data
	Vector3 *verts;
	Vector2 *tex;
	GLfloat *alpha;
	GLuint *inds;

	//Ids
	GLuint vertId;
	GLuint texId;
	GLuint alphaId;
	GLuint indsId;

	GLuint vaoId; 

	int num = 0;
	int numIndicies = 0;



	Mesh();
	~Mesh();
	Mesh(std::string filepath);	//Read from file TODO:

	//static Mesh *Triangle();
	//static Mesh *Quad();
	static Mesh *QuadInds();
	static Mesh *Plane(int x, int z, float xTexture, float yTexture);
	
	void updateVerts() { 
		glBindBuffer(GL_ARRAY_BUFFER, vertId);
		glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);
	}

	void buffer(); 
	
	int getLength() { return num; }


	

};

