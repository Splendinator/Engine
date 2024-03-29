#pragma once
#include "GL/freeglut.h"


namespace Timer {

	//Returns the time between the last call to this function and this one. Call this every frame.
	float getDelta() {
		static unsigned long long int lastTime = 0;
		float f = float(glutGet(GLUT_ELAPSED_TIME) - lastTime) / 1000;
		lastTime = glutGet(GLUT_ELAPSED_TIME);
		return f;
	}

}
