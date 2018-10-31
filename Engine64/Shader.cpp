#include "Shader.h"





Shader::Shader(std::string vert, std::string frag)
{
	programID = glCreateProgram();
	GLuint vertObj = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragObj = glCreateShader(GL_FRAGMENT_SHADER);
	char *vertText = IO::loadTextFromFile(vert);
	char *fragText = IO::loadTextFromFile(frag);

	GLint status;		//Status GL_TRUE if worked or GL_FALSE if failed.
	char error[8192];	//Buffer to hold text.

	if (!vertText) {
		std::cout << "Error reading file: " << vert << std::endl;
	}

	if (!fragText) {
		std::cout << "Error reading file: " << frag << std::endl;
	}

	
	//Vertex Shader
	glShaderSource(vertObj, 1, &vertText, NULL);
	glCompileShader(vertObj);
	glGetShaderiv(vertObj, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(vertObj, 8192, NULL, error);
		std::cout << "Error compiling vertex shader: " << error;
		programID = 0;
	}
	glAttachShader(programID, vertObj);

	
	//Fragment Shader
	glShaderSource(fragObj, 1, &fragText, NULL);
	glCompileShader(fragObj);
	glGetShaderiv(fragObj, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(fragObj, 8192, NULL, error);
		std::cout << "Error compiling fragment shader: " << error;
		programID = 0;
	}
	glAttachShader(programID, fragObj);


	glLinkProgram(programID);
	glGetShaderiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(programID, 8192, NULL, error);
		std::cout << "Error linking compiled shader: " << error;
		programID = 0;
	}

}

Shader::~Shader()
{
}
