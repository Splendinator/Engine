#include "Water.h"





Water::Water(int numX, int numZ, float textureX, float textureZ, Texture * texture, Shader * shader, GLuint *reflectionCube, float ref) : Heightmap(numX, numZ, textureX, textureZ, texture, shader)
{
	reflection = reflectionCube;
	reflectivity = ref;
}


Water::~Water()
{
}

void Water::draw()
{
	

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *reflection);

	glUniform1i(glGetUniformLocation(s->programID, "texReflect"), 3);
	glActiveTexture(GL_TEXTURE0);


	glUniform1i(glGetUniformLocation(s->programID, "reflectivity"), reflectivity);
	glActiveTexture(GL_TEXTURE0);


	Heightmap::draw();
}
