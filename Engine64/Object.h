#pragma once
#include "Mesh.h";
#include "Matrix4.h";
#include "Texture.h"
#include "Shader.h"
#include "Vector.h"
#include <vector>

class Object
{
private:
	Mesh *m;
	Texture *t;
	Shader *s; 
	std::vector <Object *> children;
	Object *parent;
	Matrix4 worldTransform = Matrix4::identity();
	Matrix4 localTransform = Matrix4::identity();
	Vector3 offset = Vector3({ 0,0,0 });

	void updateChildren();
	void updateWorldMatrix(Matrix4 m);
public:

	GLuint g;

	Object();
	Object(Mesh *mesh, Texture *texture, Shader *shader);
	~Object();


	void init();

	void draw();

	void transform(Matrix4 transform);
	void addChild(Object *o);

	void setOffset(float x, float y, float z) { offset = Vector3({ x,y,z }); transform(Matrix4::translate(x, y, z)); }

	Matrix4 getTransform() { return worldTransform * localTransform ; }
	
	friend class Rasteriser;
};

