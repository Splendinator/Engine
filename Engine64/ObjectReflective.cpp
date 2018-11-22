#include "ObjectReflective.h"




ObjectReflective::ObjectReflective(Mesh * mesh, Texture * texture, Shader * shader, GLuint rm, float r) : Object(mesh,texture,shader)
{
	reflectMap = rm;
	reflectivity = r;
}

void ObjectReflective::draw()
{


	Object::draw();
}

ObjectReflective::~ObjectReflective()
{
}
