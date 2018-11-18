#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Vector.h"

class IO
{
public:
	IO();

	//Returns pointer to file text, or nullptr if it couldn't be read.
	//TODO: Fix for windows line endings.
	//REMEMBER TO FREE MEMORY AND CHECK FOR NULLPTR
	static char *loadTextFromFile(std::string filename);


	static void readObj(std::string filePath, std::vector<Vector3> &pos, std::vector<int> &inds);


	~IO();
};

