#include "Texture.h"




Texture::Texture(char *diffuse, char *normals)
{
	/* load an image file directly as a new OpenGL texture */
	id = SOIL_load_OGL_texture
	(
		diffuse,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* check for an error during the load process */
	if (0 == id)
		std::cout << "SOIL loading error: " << SOIL_last_result();


	/* load an image file directly as a new OpenGL texture */
	normal = SOIL_load_OGL_texture
	(
		normals,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* check for an error during the load process */
	if (0 == id)
		std::cout << "SOIL loading error: " << SOIL_last_result();

}

Texture::~Texture()
{
}
