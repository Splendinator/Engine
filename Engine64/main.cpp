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
#include "PointLight.h"
#include "ObjectReflective.h"
#include "Water.h"
#include <chrono>



static const int WATER_SIZE = 127;



struct Scene {

	Object *root;
	std::vector<PointLight *> *lights;

};

char charBuffer[256];


int scene = -1;


GLuint reflectionTexture;

Rasteriser r;

//Mesh *m = Mesh::Plane(3, 3, 1.0f, 1.0f);
Mesh *m = Mesh::QuadInds();
Mesh monkey("Models/monkey.model");

Texture monkeyTex;

Texture t;
Texture waterTex;
Texture rockTex;
Texture bumpMapTex;
Texture snowTex;


Shader s;
Shader shaderTerrain;
Shader shaderReflect;


PointLight l(2000.f, Vector4({ 0.7f,0.7f,0.7f,0.4f }), Vector3({0, 1000.f, 0.f}));



///SCENE 1
Heightmap h(257, 257, 16.f,16.f,&t,&rockTex,&shaderTerrain);
Water water(WATER_SIZE, WATER_SIZE, 4.f, 4.f, &waterTex, &shaderReflect, &reflectionTexture, 0.6f);




///SCENE 2
Heightmap rootTwo(257, 257, 16.f, 16.f, &bumpMapTex, &s);
Object monkeyObj(&monkey, &monkeyTex, &s);
PointLight sceneTwoLight(50.f, Vector4({ 0.6f,0.6f,0.6f,1.0f }), Vector3({100.f,20.f,100.f}));



///SCENE 3
Heightmap h(257, 257, 16.f, 16.f, &snowTex, &rockTex, &shaderTerrain);
Water water(WATER_SIZE, WATER_SIZE, 4.f, 4.f, &waterTex, &shaderReflect, &reflectionTexture, 0.6f);





Camera cam;

const float CAMERA_SPEED = 10.0f;
const float SPRINT_SPEED = 50.f;
const float SENSITIVITY = 0.003f;


std::vector<PointLight *> lightsOne;
std::vector<PointLight *> lightsTwo;
std::vector<PointLight *> lightsThree;

Scene scenes[3];


void changeScene(int scene) {
	
	r.clear();
	r.addObject(scenes[scene].root);
	for (std::vector<PointLight *>::iterator it = scenes[scene].lights->begin(); it != scenes[scene].lights->end(); ++it) {
		r.addPointLight(*it);
	}

}

const float TRANSITION_ANIMATION_TIME = 1.0f;

float sceneChange = 0.f;
float fpsUpdate = 0.f;
float splitScreen = 1.0f;

//GAME LOOP
void gameLoop(void) {
	static float time = 0;
	double delta = Timer::getDelta();
	time += delta;
	float camSpeed = CAMERA_SPEED;
	splitScreen += delta;
	std::cout << splitScreen << std::endl;

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

	if (Input::keyDown(Input::Key('o'))) {
		if (sceneChange < time) {
			sceneChange = time + TRANSITION_ANIMATION_TIME;
			changeScene(scene = ((scene + 1) % 3));
			splitScreen = 0.0f;
		}
	}

	cam.rollYaw(Input::relativeMousePos()[0] * SENSITIVITY);
	cam.rollPitch(Input::relativeMousePos()[1] * SENSITIVITY);

	
	if (scene == 1 || splitScreen < TRANSITION_ANIMATION_TIME) {
		sceneTwoLight.pos = Vector3({ sin(time), 0.f, cos(time) }) * 40.f;
		if (scene == 1) {
			r.calculateShadowmap(sceneTwoLight.pos, -time);
		}
	}
	else
	if (scene == 0 || splitScreen < TRANSITION_ANIMATION_TIME) {
		for (int i = 0; i < WATER_SIZE; ++i) {
			for (int j = 0; j < WATER_SIZE; ++j) {
				water.height(i, j) = (sin(time + water.vert(i,j)[0] * 10.8f +  water.vert(i,j)[2] * 5.7f) * 1.2f * cos(time - water.vert(i, j)[0] * 7.8f + water.vert(i, j)[2] * 13.7f) * sin(time + water.vert(i, j)[0] * 3.8f - water.vert(i, j)[2] * 9.7f) * cos(time - water.vert(i, j)[0] * 11.8f + water.vert(i, j)[2] * 6.7f));
				
			}
		}
		water.updateHeight();
	}

	if (fpsUpdate < time) {
		sprintf_s(charBuffer, 256, "FPS %d", int(1.f / delta));
		fpsUpdate += 1.f;
	}
	r.hudText = std::string(charBuffer);
	
	
	if (splitScreen < TRANSITION_ANIMATION_TIME) {
		
		r.update(1-(splitScreen / TRANSITION_ANIMATION_TIME));
	}
	else
	r.update();



	Input::update();

	

}



int main(int argc, char** argv) {
	
	scenes[0].root = &h;
	scenes[0].lights = &lightsOne;
	scenes[1].root = &rootTwo;
	scenes[1].lights = &lightsTwo;
	scenes[2].root = &h;//TODO
	scenes[2].lights = &lightsThree;


	//INIT OPENGL/FREEGLUT
	Initialize::init(argc, argv);

	r.init();
	
	//SHADER
	s = Shader("Shaders\\scene.vert", "Shaders\\scene.frag");
	shaderTerrain = Shader("Shaders\\heightmap.vert", "Shaders\\heightmap.frag");
	shaderReflect = Shader("Shaders/water.vert", "Shaders/water.frag");

	
	t = Texture("Textures/grass.jpg");
	rockTex = Texture("Textures/rock.jpg");

	snowTex = Texture("Textures/snow.jpg");

	waterTex = Texture("Textures/water.jpg");

	monkeyTex = Texture("Models/monkey.jpg");

	bumpMapTex = Texture("Textures/ASSdiffuse.jpg", "Textures/ASSnormals.jpg");



	//REFLECTION MAP
	//GLuint reflectionMap = r.calculateReflections(Vector3({ 0, 0, 0 }));


	//OBJECT
	//o = ObjectReflective(&monkey, &monkeyTex, &s,0,0.5f);
	//o2 = Object(m, t, &s);
	//o3 = Object(m, t, &s);
	
	

	///SCENE 1


	h.init();
	//o.init();
	water.init();
	
	h.readHM("Heightmap/hm.jpg", 2048, 2048);
	h.transform(Matrix4::scale(300,0.6f,300.f));
	h.transform(Matrix4::translate(0, -50.f, 0));


	h.addChild(&water);

	water.transform(Matrix4::scale(0.3f, 1.f, 0.3f));
	water.transform(Matrix4::translate(0, 70.f, 0));

	lightsOne.push_back(&l);
	
	


	///SCENE 2
	rootTwo.init();
	monkeyObj.init();

	rootTwo.transform(Matrix4::scale(2000.f, 1.f, 2000.f));
	rootTwo.transform(Matrix4::translate(0, -20.f, 0));
	rootTwo.addChild(&monkeyObj);

	monkeyObj.transform(Matrix4::scale(0.004f, 8.f, 0.004f));
	monkeyObj.transform(Matrix4::translate(0.0f, 10.f, 0.0f));

	lightsTwo.push_back(&sceneTwoLight);




	
	

	//MVP
	r.bindCamera(&cam);
	r.setProjection(Matrix4::Perspective(0.01f,10000,PI/2.f, float(glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT)));

	glutDisplayFunc(gameLoop);
	glutIdleFunc(gameLoop);

	Input::setup();




	reflectionTexture = SOIL_load_OGL_cubemap(
		"Textures/interstellar_+x.png",
		"Textures/interstellar_-x.png",
		"Textures/interstellar_+y.png",
		"Textures/interstellar_-y.png",
		"Textures/interstellar_+z.png",
		"Textures/interstellar_-z.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);



	

	glutMainLoop();

	


	return 0;

}



