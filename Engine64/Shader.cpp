#include "Shader.h"



GLuint Shader::compileProgram(std::string vert, std::string frag) {

	GLuint program = glCreateProgram();
	GLuint vertObj = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragObj = glCreateShader(GL_FRAGMENT_SHADER);
	char *vertText = IO::loadTextFromFile(vert);
	char *fragText = IO::loadTextFromFile(frag);
	GLint status;
	
	if (!vertText || !fragText) {
		std::cout << "Error reading file(s)" << std::endl;
		return 0;
	}
	
	//Vertex Shader
	glShaderSource(vertObj, 1, &vertText, NULL);
	glCompileShader(vertObj);
	glGetShaderiv(vertObj,GL_COMPILE_STATUS,&status);
	if (status != GL_TRUE) {
		char error[8192];
		glGetShaderInfoLog(vertObj, 8192, NULL, error);
		std::cout << error;
		return 0;
	}
	glAttachShader(program, vertObj);

	//Fragment Shader
	glShaderSource(fragObj, 1, &fragText, NULL);
	glCompileShader(fragObj);
	glGetShaderiv(fragObj, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char error[8192];
		glGetShaderInfoLog(fragObj, 8192, NULL, error);
		std::cout << error;
		return 0;
	}
	glAttachShader(program, fragObj);

	glLinkProgram(program);
	glUseProgram(program);



	return 0;
}

Shader::Shader()
{
}


Shader::~Shader()
{
}
