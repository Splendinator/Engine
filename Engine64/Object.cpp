#include "Object.h"




Object::Object()
{
	m = new Mesh();      
}

Object::Object(Mesh *mesh, Texture *texture, Shader *shader)
{
	m = mesh;
	t = texture;

	s = shader;

}

Object::~Object()
{

}

void Object::init()
{
	//Set default atributes.
	glBindAttribLocation(s->programID, VERTEX_ID, "pos");
	glBindAttribLocation(s->programID, TEXTURE_ID, "tex");

	m->buffer();
}

void Object::draw() {
	glUseProgram(s->programID);
	
	glBindTexture(GL_TEXTURE_2D, t->id);

	glBindVertexArray(m->vaoId);
	glDrawArrays(GL_TRIANGLES, 0, m->getLength());

	
	
}

inline void Object::updateChildren() {
	for (std::vector<Object *>::iterator it = children.begin(); it != children.end(); ++it)
		(*it)->updateWorldMatrix(getTransform());
};

inline void Object::updateWorldMatrix(Matrix4 m)
{
	worldTransform = m;
	updateChildren();
}


void Object::transform(Matrix4 transform)
{
	localTransform *= Matrix4::translate(-offset[0], -offset[1], -offset[2]);
	localTransform *= transform;
	localTransform *= Matrix4::translate(offset[0], offset[1], offset[2]);
	updateChildren();
}

void Object::addChild(Object * o)
{
	children.push_back(o);
	o->updateWorldMatrix(getTransform());
}
