#include "Shader.h"



GLuint Shader::compileProgram(std::string vert, std::string frag) {

	GLuint program = glCreateProgram();
	GLuint vertObj = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragObj = glCreateShader(GL_FRAGMENT_SHADER);
	char *vertText = IO::loadTextFromFile(vert);		
	char *fragText = IO::loadTextFromFile(frag);

	GLint status;		//Status GL_TRUE if worked or GL_FALSE if failed.
	char error[8192];	//Buffer to hold text.
	
	if (!vertText || !fragText) {
		std::cout << "Error reading file(s)" << std::endl;
		return 0;
	}
	
	//Vertex Shader
	glShaderSource(vertObj, 1, &vertText, NULL);
	glCompileShader(vertObj);
	glGetShaderiv(vertObj,GL_COMPILE_STATUS,&status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(vertObj, 8192, NULL, error);
		std::cout << "Vertex: "<< error;
	}
	glAttachShader(program, vertObj);

	//Fragment Shader
	glShaderSource(fragObj, 1, &fragText, NULL);
	glCompileShader(fragObj);
	glGetShaderiv(fragObj, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(fragObj, 8192, NULL, error);
		std::cout << "Fragment: " << error;
		return 0;
	}
	glAttachShader(program, fragObj);

	glLinkProgram(program);
	glGetShaderiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(program, 8192, NULL, error);
		std::cout << "Linking: " << error;
		return 0;
	}
	glUseProgram(program);



	return program;
}

Shader::Shader()
{
}


Shader::~Shader()
{
}
