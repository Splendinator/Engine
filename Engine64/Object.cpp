#include "Object.h"




Object::Object()
{
	m = nullptr;
	t = nullptr;
	s = nullptr;
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
	glBindAttribLocation(s->programID, VERTEX_ID, "position");
	glBindAttribLocation(s->programID, TEXTURE_ID, "texCoords");
	glBindAttribLocation(s->programID, ALPHA_ID, "alpha");
	glBindAttribLocation(s->programID, NORMAL_ID, "norms");
	glBindAttribLocation(s->programID, TANGENT_ID, "tangents");
	glBindAttribLocation(s->programID, BINORMAL_ID, "binorms");

	if (!m->vertId) {
		m->buffer();
	}

}

void Object::draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->id);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t->normal);

	glBindVertexArray(m->vaoId);
	if (m->inds)
		glDrawElements(GL_TRIANGLES, m->numIndicies, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m->getLength());

	
	
}

