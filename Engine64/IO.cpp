#include "IO.h"
#include <fstream>
#include <iostream>

using namespace std;

char * IO::loadTextFromFile(string filename) {
	fstream fs;
	int length;
	char *text;
	
	fs.open(filename, fstream::in);
	if (!fs) {
		cout << "Error reading " << filename << endl;
		return nullptr;
	}
	
	//Length of file
	fs.seekg(0, fs.end);
	length = fs.tellg();
	text = new char[length+1];
	
	//Read whole file
	fs.seekg(0, fs.beg);
	fs.read(text, length);
	fs.close();

	//Null terminator at end (This is why we do 'length+1' when calculating text array size.)
	text[length] = '\0';
	
	return text;
}


IO::IO()
{
}



IO::~IO()
{
}
