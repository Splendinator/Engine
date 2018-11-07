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

