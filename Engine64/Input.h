#pragma once
#include <iostream>
#include "GL\freeglut.h"
#include "Vector.h"







namespace Input {

	const static int SPECIAL = 256;
	const static int MOUSE = 512;

	enum Key {
		KEYBOARD_ESC = 27,
		KEYBOARD_SPACE = 32,
		KEYBOARD_A = 97,
		KEYBOARD_D = 100,
		KEYBOARD_S = 115,
		KEYBOARD_W = 119,

		KEYBOARD_SHIFT = SPECIAL + 112,
		KEYBOARD_CTRL = SPECIAL + 114,
	};





	bool keyDown(Key key);

	//TODO: Inline these, requires making input a class to private away read only variables.
	Vector2i relativeMousePos();
	Vector2i absoluteMousePos();

	//Called once per frame to allow us to check wether a button was pressed this frame specifically. Must be called after you have handled any input.
	void update();

	//Setup glut key reading
	void setup();

	




}

