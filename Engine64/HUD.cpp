#include "HUD.h"



HUD::HUD()
{
	FT_Library library;
	bool error = FT_Init_FreeType(&library);
	if (error)
		std::cout << "Error initialising freetype";
	
}


HUD::~HUD()
{
}
