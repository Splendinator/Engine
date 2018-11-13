#pragma once
#include "Object.h"
 
//TODO: Redo if you plan on creating or editing these at run-time (this is optimised as a "static heightmap" that wont change much)
class Heightmap :
	public Object
{
private:
	int sizeX, sizeZ;

public:

	//Heightmap() : Object() {};
	Heightmap(int numX, int numZ, float textureX, float textureZ, Texture *texture, Shader *shader);
	~Heightmap() { delete m; Object::~Object(); };

	float &height(int x, int z) { return m->verts[(x + z*sizeZ)][1]; };

	void updateHeight() { m->calculateNormals(); m->updateVerts(); }

	float getSizeX() { return sizeX; }
	float getSizeZ() { return sizeZ; }


};

