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

Mesh *m = Mesh::Quad();

Texture *t;

Shader s;

Object o;

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
	if (Input::keyDown(Input::Key('q'))) {
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


	
	//SHADER
	s = Shader("Shaders\\basic.vert", "Shaders\\basic.frag");
	
	
	t = new Texture("Textures/checkerboard.jpg");

	//OBJECT
	o = Object(m, t, &s);
	o.transform *= Matrix4::scale(100, 100, 1);
	o.transform *= Matrix4::rotationX(PI / 2);
	o.transform *= Matrix4::translate(0, -10, 0);
	

	//THIS STUFF SHOULD BE IN A SCENE NODE INITIALISE RECURSIVE THING.
	o.init();


	o.transform *= Matrix4::translate(0, 0, -5);
	r.addObject(&o);

	//MVP
	r.bindCamera(&cam);
	r.setProjection(Matrix4::Perspective(0.01f,1000,PI, float(glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT)));

	glutDisplayFunc(gameLoop);
	glutIdleFunc(gameLoop);

	Input::setup();

	glutMainLoop();

	return 0;

}



