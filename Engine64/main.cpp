#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include <string.h>
#include "Mesh.h"
#include "Object.h"
#include "Rasteriser.h"
#include "Vector.h"
#include "Input.h"
#include "Initialize.h"
#include "Timer.h"



Rasteriser r;

Mesh *m = Mesh::Triangle();

Object o = Object(m);
Object o2 = Object(m);

Shader s;

Camera cam;

const float CAMERA_SPEED = 2.0f;
const float SPRINT_SPEED = 5.0f;
const float SENSITIVITY = 0.003f;


//GAME LOOP
void gameLoop(void) {
	float delta = Timer::getDelta();
	float camSpeed = CAMERA_SPEED;

	if (Input::keyDown(Input::KEYBOARD_ESC)) {
		exit(0);
	}
	if (Input::keyDown(Input::KEYBOARD_SHIFT)) {
		camSpeed = SPRINT_SPEED;
	}
	if (Input::keyDown(Input::KEYBOARD_W)) {
		cam.move(cam.foward() * delta * camSpeed);
	}
	if (Input::keyDown(Input::KEYBOARD_A)) {
		cam.move(cam.right() * -delta * camSpeed);
	}
	if (Input::keyDown(Input::KEYBOARD_S)) {
		cam.move(cam.foward() * -delta * camSpeed);
	}
	if (Input::keyDown(Input::KEYBOARD_D)) {
		cam.move(cam.right() * delta * camSpeed);
	}
	if (Input::keyDown(Input::KEYBOARD_SPACE)) {
		cam.move(cam.up() * delta * camSpeed);
	}
	if (Input::keyDown(Input::KEYBOARD_CTRL)) {
		cam.move(cam.up() * -delta * camSpeed);
	}

	cam.rollYaw(Input::relativeMousePos()[0] * SENSITIVITY);
	cam.rollPitch(Input::relativeMousePos()[1] * SENSITIVITY);


	r.update();
	Input::update();
}



int main(int argc, char** argv) {

	
	//INIT OPENGL/FREEGLUT
	Initialize::init(argc, argv);

	//OBJECTS
	o.transform *= Matrix4::translate(0, 0, -5);
	r.addObject(&o);
	
	//SHADER
	s = Shader("Shaders\\basic.vert", "Shaders\\basic.frag");
	r.setShader(s);
	
	//MVP
	r.bindCamera(&cam);
	r.setProjection(Matrix4::Perspective(0.01f,1000,100,1));

	glutDisplayFunc(gameLoop);
	glutIdleFunc(gameLoop);

	Input::setup();

	glutMainLoop();

	return 0;

}



