#include "Texture.h"



Texture::Texture(char * filepath)
{
	/* load an image file directly as a new OpenGL texture */
	id = SOIL_load_OGL_texture
	(
		filepath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	/* check for an error during the load process */
	if (0 == id)
		std::cout << "SOIL loading error: " << SOIL_last_result();

	std::cout << id;

}

Texture::~Texture()
{
}
