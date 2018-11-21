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
#include "Heightmap.h"





Rasteriser r;

//Mesh *m = Mesh::Plane(3, 3, 1.0f, 1.0f);
Mesh *m = Mesh::QuadInds();
Mesh monkey("Models/monkey.model");

Texture monkeyTex;

Texture t;
Texture waterTex;
Texture rockTex;
Texture bumpMapTex;

Shader s;
Shader shaderTerrain;

Object o;
Object o2;
Object o3;


Heightmap h(257, 257, 16.f,16.f,&t,&rockTex,&shaderTerrain);
Heightmap water(257, 257, 4.f, 4.f, &bumpMapTex, &s);

Camera cam;

const float CAMERA_SPEED = 10.0f;
const float SPRINT_SPEED = 50.f;
const float SENSITIVITY = 0.003f;


//GAME LOOP

void gameLoop(void) {
	static float time = 0;
	float delta = Timer::getDelta();
	time += delta;
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

	o.transform(Matrix4::translate(2 * delta, 0, 0));

	r.update();
	Input::update();

	

}



int main(int argc, char** argv) {
	


	//INIT OPENGL/FREEGLUT
	Initialize::init(argc, argv);

	r.init();
	
	//SHADER
	s = Shader("Shaders\\shadow.vert", "Shaders\\shadow.frag");
	shaderTerrain = Shader("Shaders\\heightmap.vert", "Shaders\\heightmap.frag");
	
	t = Texture("Textures/grass.jpg");
	rockTex = Texture("Textures/rock.jpg");

	waterTex = Texture("Textures/water.jpg");

	monkeyTex = Texture("Models/monkey.jpg");

	bumpMapTex = Texture("Textures/ASSdiffuse.jpg", "Textures/ASSnormals.jpg");


	//OBJECT
	o = Object(&monkey, &monkeyTex, &s);
	//o2 = Object(m, t, &s);
	//o3 = Object(m, t, &s);
	
	

	//THIS STUFF SHOULD BE IN A SCENE NODE INITIALISE RECURSIVE THING.
	h.init();
	o.init();
	water.init();
	//o2.init();
	//o3.init();


	//o.transform(Matrix4::rotationZ(delta));
	h.readHM("Heightmap/hm.jpg", 2048, 2048);
	h.transform(Matrix4::scale(300,0.6f,300.f));
	h.transform(Matrix4::translate(0, -50.f, 0));

	water.transform(Matrix4::scale(300, 1.f, 300.f));
	water.transform(Matrix4::translate(0, -10.f, 0));
	//water.transform(Matrix4::rotationX(PI/50));

	o.transform(Matrix4::scale(5.f, 5.f, 5.f));
	o.transform(Matrix4::translate(0, 0, -10.f));
	//o2.transform(Matrix4::translate(0, 0, -3));
	//o3.transform(Matrix4::translate(0, 0, -3));
	
	///TODO:fix
	//h.addChild(water);

	//o.addChild(&o2);
	//o2.addChild(&o3);

	//r.addObject(&h);
	r.addObject(&water);
	r.addObject(&o);
	//r.addObject(&o2);
	//r.addObject(&o3);
	

	//MVP
	r.bindCamera(&cam);
	r.setProjection(Matrix4::Perspective(0.01f,1000,PI/2.f, float(glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT)));

	glutDisplayFunc(gameLoop);
	glutIdleFunc(gameLoop);

	Input::setup();


	

	glutMainLoop();

	


	return 0;

}



