#pragma once
#include "Mesh.h";
#include "Matrix4.h";
#include "Texture.h"
#include "Shader.h"
#include "Vector.h"
#include "Camera.h"
#include <vector>

class Object
{
protected:
	Mesh *m;
	Texture *t;
	Shader *s; 
	std::vector <Object *> children;
	Object *parent;
	Matrix4 worldTransform = Matrix4::identity();
	Matrix4 localTransform = Matrix4::identity();
	Matrix4 transformMatrix;
	//Vector3 offset = Vector3({ 0,0,0 });

	float cameraDist;

	void updateChildren();
	void updateWorldMatrix(Matrix4 m);
public:


	GLuint g;

	Object();
	Object(Mesh *mesh, Texture *texture, Shader *shader);
	virtual ~Object();

	void calcCameraDist(Camera *c) { cameraDist = 
		((transformMatrix[3] - c->getTransform()[3]) * (transformMatrix[3] - c->getTransform()[3]))
		+ ((transformMatrix[7] - c->getTransform()[7]) * (transformMatrix[7] - c->getTransform()[7]))
		+ ((transformMatrix[11] - c->getTransform()[11]) * (transformMatrix[11] - c->getTransform()[11]));
	}

	void init();

	virtual void draw();

	void transform(Matrix4 transform);
	void addChild(Object *o);

	//void setOffset(float x, float y, float z) { offset = Vector3({ x,y,z }); transform(Matrix4::translate(x, y, z)); }

	Matrix4 getTransform() { return transformMatrix; }


	static bool compareCameraDistLT(const Object *a, const Object *b) { return a->cameraDist < b->cameraDist; }
	static bool compareCameraDistGT(const Object *a, const Object *b) { return a->cameraDist > b->cameraDist; }

	
	friend class Rasteriser;
};



inline void Object::updateChildren() {
	for (std::vector<Object *>::iterator it = children.begin(); it != children.end(); ++it)
		(*it)->updateWorldMatrix(getTransform());
};

inline void Object::updateWorldMatrix(Matrix4 m)
{
	worldTransform = m;
	transformMatrix = worldTransform * localTransform;
	updateChildren();
}


inline void Object::transform(Matrix4 transform)
{
	Matrix4 t = localTransform;

	//TODO: Maybe remove outer two and handle scaling complications.
	localTransform *= Matrix4::translate(t[3], t[7], t[11]) * transform * Matrix4::translate(-t[3], -t[7], -t[11]);

	transformMatrix = worldTransform * localTransform;
	
	updateChildren();
}

inline void Object::addChild(Object * o)
{
	children.push_back(o);
	o->updateWorldMatrix(getTransform());
}
