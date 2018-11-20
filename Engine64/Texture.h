#pragma once
#include "SOIL/SOIL.h"
#include "GL/glew.h"
#include <iostream>



class Texture
{

public:
	GLuint id;
	GLuint normal;

	Texture(char *filepath);
	Texture(char *diffuse, char *normals);
	Texture() { id = 0; };
	Texture(GLuint name) { id = name; };
	~Texture();
};

