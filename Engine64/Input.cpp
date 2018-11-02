#include "Input.h"



//Moving mouse callback
void handleMotion(int x, int y) {
	static int lastx = x;	//Static so retain value between func calls.
	static int lasty = y;

	

	lastx = x;
	lasty = y;
}


void handleKeyDown(unsigned char c, int x, int y)
{
	std::cout << "Key: " << int(c) << std::endl;
}


void handleSpecial(int c, int x, int y)
{
	std::cout << "Special: " << c << std::endl;
}

void handleMouse(int button, int state, int x, int y)
{
	std::cout << "Mouse: " << button << " " << (state? "R" : "P") << std::endl;

}


void Input::setup() { 

	glutKeyboardFunc(handleKeyDown);
	glutSpecialFunc(handleSpecial);

	glutPassiveMotionFunc(handleMotion);
	glutMotionFunc(handleMotion);

	glutMouseFunc(handleMouse);
}