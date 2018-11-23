#pragma once
#include "GL/glew.h"
#include <string>
#include <iostream>
#include "IO.h"

class Shader
{



public:

	GLuint programID = 0;


	Shader() {};

	//Compiles shaders pointed to by filepaths.
	Shader(std::string vert, std::string frag);
	Shader(std::string vert, std::string frag, std::string geo);



	~Shader();

};

