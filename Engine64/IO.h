#pragma once
#include <string>

class IO
{
public:
	IO();

	//Returns pointer to file text, or nullptr if it couldn't be read.
	//TODO: Fix for windows line endings.
	//REMEMBER TO FREE MEMORY AND CHECK FOR NULLPTR
	static char * loadTextFromFile(std::string filename);

	~IO();
};

