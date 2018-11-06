#pragma once
#include "Mesh.h";
#include "Matrix4.h";
#include "Texture.h"
#include "Shader.h"
#include <vector>

class Object
{
private:
	Mesh *m;
	Texture *t;
	Shader *s; 
	std::vector <Object *> children;
	Object *parent;

public:
	Matrix4 localTransform = Matrix4::identity();
	Matrix4 worldTransform = Matrix4::identity();

	GLuint g;

	Object();
	Object(Mesh *mesh, Texture *texture, Shader *shader);
	~Object();


	void init();

	void draw();
	
	friend class Rasteriser;
};

