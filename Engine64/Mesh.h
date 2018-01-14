#pragma once
#include <string>
#include <iostream>
#include <GL\glew.h>
class Mesh
{
private:


public:

	float *verts;
	int num = 0;

	Mesh();
	~Mesh();
	Mesh(std::string filepath);	//Read from file TODO:

	void buffer() const;
	int getLength() { return num / 3; }


	

};

