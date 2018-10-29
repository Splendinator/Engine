#pragma once
#include <string>
#include <iostream>
#include <GL\glew.h>
class Mesh
{
private:


public:

	float *verts;
	float *cols;
	int num = 0;

	Mesh();
	~Mesh();
	Mesh(std::string filepath);	//Read from file TODO:

	static Mesh *Triangle();
	static Mesh *Cube();

	void buffer() const;
	
	int getLength() { return num / 3; }


	

};

