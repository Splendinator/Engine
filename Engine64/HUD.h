#pragma once

#include <iostream>
#include "GL\glew.h"

#include "ft2build.h"
#include FT_FREETYPE_H;
#include FT_GLYPH_H

class HUD
{
public:
	static const int PIXEL_SIZE = 64;
	static const int CHARS = 255;

	char *temp = new char[PIXEL_SIZE * PIXEL_SIZE];

	FT_Library library;
	FT_Face     face;

	FT_UInt glyphIndex[CHARS];

	GLuint textures[CHARS];

	HUD();
	~HUD();

	void init();
};

