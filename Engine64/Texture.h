#pragma once
#include "SOIL/SOIL.h"
#include "GL/glew.h"
#include <iostream>



class Texture
{

public:
	GLuint id;

	Texture(char *filepath);
	~Texture();
};

