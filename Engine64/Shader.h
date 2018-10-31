#pragma once
#include "GL/glew.h"
#include <string>
#include <iostream>
#include "IO.h"

class Shader
{
private:
	GLuint programID = 0;

public:
	
	Shader() {};

	//Compiles shaders pointed to by filepaths.
	Shader(std::string vert, std::string frag);



	~Shader();

	friend class Rasteriser;
};

