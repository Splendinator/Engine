#pragma once
#include "Mesh.h";
#include "Matrix4.h";

class Object
{
private:
	Mesh *m;
	GLint transformId;

public:
	Matrix4 transform = Matrix4::identity();

	Object();
	Object(Mesh *mesh);
	~Object();

	void draw();
};

