#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include "IO.h"

class Shader
{
public:
	Shader();

	static GLuint compileProgram(std::string vert, std::string frag);

	~Shader();
};

