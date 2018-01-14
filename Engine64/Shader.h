#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include "IO.h"

class Shader
{
public:
	Shader();

	//Compiles shaders pointed to by filepaths. Returns program ID.
	static GLuint compileProgram(std::string vert, std::string frag);

	~Shader();
};

