#include "Heightmap.h"

Heightmap::Heightmap(int numX, int numZ, float textureX, float textureZ, Texture * texture, Shader * shader) : Object(nullptr, texture, shader)
{
	sizeX = numX;
	sizeZ = numZ;
	m = Mesh::Plane(numX, numZ, textureX, textureZ);
}
