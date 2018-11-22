#pragma once
#include "Heightmap.h"
class Water :
	public Heightmap
{
public:

	GLuint *reflection;
	float reflectivity;

	
	Water(int numX, int numZ, float textureX, float textureZ, Texture *texture, Shader *shader, GLuint *reflectionCube, float reflectivity);
	~Water();

	void draw();
};

