#include "Input.h"



bool keysDown[1024];	//Is a key currently held down?
Vector2i mousePos;	//Mouse position in pixels.
Vector2i relMousePos;	//Mouse position relative to last frame in pixels.

unsigned char mapping[256];	//Maps CTRL, SHIFT, and ALT modifiers to their original values before modifiers. SHIFT + A is 67 in ASCII, and hence passed by the handleKeyboardDown function,  but we want the system to register 'a' (97 in ASCII), the same as if shift wasn't held at all.


//Moving mouse callback
void handleMotion(int x, int y) {
	static int lastx = x;	//Static so retain value between func calls.
	static int lasty = y;

	mousePos = Vector2i({ x,y });

	lastx = x;
	lasty = y;

}


void handleKeyDown(unsigned char c, int x, int y)
{

	keysDown[mapping[c]] = true;
	//std::cout << "KEY DOWN: " << int(mapping[c]) << std::endl;
}

void handleKeyUp(unsigned char c, int x, int y)
{
	keysDown[mapping[c]] = false;
	//std::cout << "KEY UP: " << int(mapping[c]) << std::endl;
}


void handleSpecialUp(int c, int x, int y)
{
	keysDown[Input::SPECIAL + c] = false;
	//std::cout << "SPECIAL UP: " << c << std::endl;
}

void handleSpecialDown(int c, int x, int y)
{
	keysDown[Input::SPECIAL + c] = true;
	//std::cout << "SPECIAL DOWN: " << c << std::endl;
}

void handleMouse(int button, int state, int x, int y)
{
	keysDown[Input::MOUSE + button] = !state;
}


void initMapping() {
	//Init normally
	for (int i = 0; i < 255; ++i) {
		mapping[i] = i;
	}

	//CTRL + LETTER MODIFIER
	for (int i = 1; i < 26; ++i) {
		mapping[i] = i + 96;
	}

	//SHIFT + LETTER MODIFIER
	for (int i = 65; i <= 90; ++i) {
		mapping[i] = i + 32;
	}

	//TODO: SHIFT + PUNCTUATION
	//TODO: CTRL + PUNCTUATION


}

void Input::setup() { 

	glutKeyboardFunc(handleKeyDown);
	glutSpecialFunc(handleSpecialDown);

	glutKeyboardUpFunc(handleKeyUp);
	glutSpecialUpFunc(handleSpecialUp);

	//Mouse Movement
	glutPassiveMotionFunc(handleMotion);
	glutMotionFunc(handleMotion);

	glutMouseFunc(handleMouse);

	glutIgnoreKeyRepeat(true);

	initMapping();
}



bool Input::keyDown(Key key)
{
	return keysDown[key];
}

Vector2i Input::relativeMousePos()
{
	return Vector2i({ mousePos[0] - glutGet(GLUT_WINDOW_WIDTH) / 2, mousePos[1] - glutGet(GLUT_WINDOW_HEIGHT) / 2 });
	
}

Vector2i Input::absoluteMousePos()
{
	return mousePos;
}

void Input::update()
{
	//if(mouseVisible)
	mousePos = Vector2i({ glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2 });
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
}

//TODO mouseVisible(false);














