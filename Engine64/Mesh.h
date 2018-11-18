#pragma once
#include "GL/glew.h"
#include "Vector.h"
#include <string>
#include <iostream>
#include <vector>
#include "IO.h"

enum ID {VERTEX_ID, TEXTURE_ID, ALPHA_ID, NORMALS_ID, INDICIES_ID};

class Mesh
{
private:


public:

	//Data
	Vector3 *verts;
	Vector2 *tex;
	GLfloat *alpha;
	GLuint *inds;
	Vector3 *normals;

	//Ids
	GLuint vertId;
	GLuint texId;
	GLuint alphaId;
	GLuint indsId;
	GLuint normalId;

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
		//glEnableVertexAttribArray(vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, vertId);
		glBufferData(GL_ARRAY_BUFFER, num * sizeof(Vector3), verts, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, normalId);
		glBufferData(GL_ARRAY_BUFFER, num * sizeof(Vector3), normals, GL_STATIC_DRAW);
	}

	void calculateNormals();

	void buffer(); 
	
	int getLength() { return num; }


	

};

