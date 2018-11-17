#include "Heightmap.h"

Heightmap::Heightmap(int numX, int numZ, float textureX, float textureZ, Texture * texture, Shader * shader) : Object(nullptr, texture, shader)
{
	sizeX = numX;
	sizeZ = numZ;
	m = Mesh::Plane(numX, numZ, textureX, textureZ);
}

//TODO: Tesselation
void Heightmap::readHM( std::string heightmap, int picWidth, int picHeight)
{
	unsigned char *data;
	data = SOIL_load_image("Heightmap/hm.jpg", &picWidth, &picHeight, 0, SOIL_LOAD_RGB);
	float pctX, pctY, dx, dy;

	for (int i = 0; i < sizeX; ++i) {
		for (int j = 0; j < sizeZ; ++j) {
			pctX = float(i) / (sizeX / float(picWidth - 1));
			pctY = float(j) / (sizeZ / float(picHeight - 1));
			dx = pctX - int(pctX);
			dy = pctY - int(pctY);
			
			height(i, j) =
				256 - (
					  data[3 * ((int(pctX)) * picHeight + int(pctY))] * (1 - dx)		* (1 - dy)
					+ data[3 * ((int(pctX + 1)) * picHeight + int(pctY))] * dx			* (1 - dy)
					+ data[3 * ((int(pctX)) * picHeight + int(pctY + 1))] * (1 - dx)	* dy
					+ data[3 * ((int(pctX + 1)) * picHeight + int(pctY + 1))] * dx		* dy);
		}
		
	}

	updateHeight();
	delete data;
}
