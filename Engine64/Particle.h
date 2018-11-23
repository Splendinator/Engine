#pragma once
#include "Object.h"
#include <vector>

class Particle :
	public Object
{
public:

	GLuint vao;
	GLuint vbo;
	
	Vector3 particle;
	float size;

	Particle(Texture *texture, Shader *shader, float particleSize) : Object(Mesh::Point(), texture, shader) { size = particleSize; };

	void draw();
	void buffer();

	~Particle();
};

