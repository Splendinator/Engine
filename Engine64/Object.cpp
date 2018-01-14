#include "Object.h"


Object::Object()
{
	m = new Mesh();
}

Object::Object(Mesh *mesh)
{
	m = mesh;
}


Object::~Object()
{
	delete m;
}

void Object::Draw() {
	
	m->buffer();	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, m->getLength(), GL_FLOAT, GL_FALSE, 0, 0);
	glUniformMatrix4fv(transformId, 1,GL_FALSE, &transform.value[0]);

}

void Object::LinkProgram(GLuint program) {

	transformId = glGetUniformLocation(program,"transform");

}
