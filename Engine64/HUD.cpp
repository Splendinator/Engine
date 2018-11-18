#include "HUD.h"



HUD::HUD()
{
	bool error = FT_Init_FreeType(&library);
	if (error)
		std::cout << "Error initialising freetype";

	
	error = FT_New_Face(library,
		"Fonts/cour.ttf",
		0,
		&face);
	if (error)
		std::cout << "Error reading font";

	FT_Set_Char_Size(
		face,    /* handle to face object           */
		0,       /* char_width in 1/64th of points  */
		PIXEL_SIZE * 64,   /* char_height in 1/64th of points */
		300,     /* horizontal device resolution    */
		300);   /* vertical device resolution      */

	FT_Set_Pixel_Sizes(
		face,   /* handle to face object */
		PIXEL_SIZE,      /* pixel_width           */
		PIXEL_SIZE);   /* pixel_height          */

	for (int i = 0; i < CHARS; ++i) {
		glyphIndex[i] = FT_Get_Char_Index(face, i);
	}

}

void HUD::init() {

	glCreateTextures(GL_TEXTURE_2D, CHARS, textures);

	FT_Glyph   glyph;
	FT_BBox  bbox;
	int width, height;

	for (int i = 0; i < CHARS; ++i) {
		FT_Load_Glyph(
			face,          /* handle to face object */
			glyphIndex[i],   /* glyph index           */
			FT_LOAD_DEFAULT);  /* load flags, see below */

		FT_Render_Glyph(face->glyph,   /* glyph slot  */
			FT_RENDER_MODE_NORMAL); /* render mode */

		
		





		for (int j = 0; j < face->glyph->bitmap.rows; ++j) {
			//memcpy(temp + (face->glyph->bitmap.width * j), face->glyph->bitmap.buffer + face->glyph->bitmap_left + ((face->glyph->bitmap.width + face->glyph->bitmap_left) * j), face->glyph->bitmap.width);
		}



		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		int te = face->glyph->bitmap.width % 4;
		//std::cout << te << " " << face->glyph->bitmap.width << " " << int(i) << " " << width << " " << height << std::endl;
		//std::cout << face->glyph->bitmap_left << " " << face->glyph->bitmap_top << " " << int(i) << std::endl;
		//std::cout << face->glyph->bitmap.width << " " << face->glyph->bitmap.rows << " " << std::endl;


		std::cout << face->glyph->bitmap.width << " " << face->glyph->bitmap.pitch << " " << face->glyph->metrics.height << std::endl;
		std::cout << int(face->glyph->bitmap.pixel_mode) << " " << ft_pixel_mode_grays << std::endl;

		//glTexImage2D(GL_TEXTURE_2D,
		//	0,
		//	GL_RED,
		//	(face->glyph->metrics.width),
		//	face->glyph->metrics.height,
		//	0,
		//	GL_RED,
		//	GL_UNSIGNED_BYTE,
		//	face->glyph->bitmap.buffer);
		
			
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RED,
			(face->glyph->bitmap.pitch),
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);
		
		
	}



}



HUD::~HUD()
{
}
