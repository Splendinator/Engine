#pragma once
#include "Object.h"
class ObjectReflective :
	public Object
{



public:


	GLuint reflectMap;
	float reflectivity;


	ObjectReflective() {};
	ObjectReflective(Mesh *mesh, Texture *texture, Shader *shader, GLuint reflectMap, float reflectivity);

	

	void draw();


	~ObjectReflective();
};

