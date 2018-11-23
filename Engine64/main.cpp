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
#include "Particle.h"
#include <chrono>
#include <random>



static const int MAX_SNOWFLAKES = 128;
static const int WATER_SIZE = 257;



struct Scene {

	Object *root;
	Object *transparent;
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
Texture snowflakeTex;

Texture t;
Texture waterTex;
Texture rockTex;
Texture bumpMapTex;
Texture snowTex;
Texture iceTex;


Shader s;
Shader shaderTerrain;
Shader shaderReflect;
Shader shaderParticle;

PointLight lightArray[132];

PointLight l(1500.f, Vector4({ 0.7f,0.7f,0.7f,1.0f }), Vector3({0, 1000.f, 0.f}));



///SCENE 1

Heightmap h(257, 257, 16.f,16.f,&t,&rockTex,&shaderTerrain);
Water water(WATER_SIZE, WATER_SIZE, 1.f, 1.f, &waterTex, &shaderReflect, &reflectionTexture, 0.4f);




///SCENE 2

Heightmap rootTwo(257, 257, 16.f, 16.f, &bumpMapTex, &s);
Object monkeyObj(&monkey, &monkeyTex, &s);
PointLight sceneTwoLight(50.f, Vector4({ 0.6f,0.6f,0.6f,1.0f }), Vector3({100.f,20.f,100.f}));



///SCENE 3

Heightmap snowMap(257, 257, 16.f, 16.f, &snowTex, &rockTex, &shaderTerrain);
Water ice(2, 2, 8.f, 8.f, &iceTex, &shaderReflect, &reflectionTexture, 0.1f);
Particle snowParticle(&snowflakeTex, &shaderParticle, 10.f);
Particle snowParticleTwo(&snowflakeTex, &shaderParticle, 5.f);





Camera cam;

const float CAMERA_SPEED = 10.0f;
const float SPRINT_SPEED = 50.f;
const float SENSITIVITY = 0.003f;
const float AUTO_CHANGE = 5.f;


std::vector<PointLight *> lightsOne;
std::vector<PointLight *> lightsTwo;
std::vector<PointLight *> lightsThree;


std::vector<Particle> snowflakes;



Scene scenes[3];


void changeScene(int scene) {

	r.clear();
	r.addObject(scenes[scene].root);
	for (std::vector<PointLight *>::iterator it = scenes[scene].lights->begin(); it != scenes[scene].lights->end(); ++it) {
		r.addPointLight(*it);
	}
	if (scenes[scene].transparent) {
		r.addTransparent(scenes[scene].transparent);
	}
}

const float TRANSITION_ANIMATION_TIME = 1.0f;
const float PP_RESET_TIME = 1.0f;

float sceneChange = 0.f;
float fpsUpdate = 0.f;
float splitScreen = 1.0f;
float ppChange;

float fps_Time_Acc = 0.f;
int counter = 0;

//GAME LOOP
void gameLoop(void) {
	static float time = 0;
	double delta = Timer::getDelta();
	time += delta;


	float camSpeed = CAMERA_SPEED;
	splitScreen += delta;

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
	if (Input::keyDown(Input::Key('p'))) {
		if (ppChange < time) {
			ppChange = time + PP_RESET_TIME;
			r.postProcessEffect = (r.postProcessEffect + 1) % 4;
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
	if (scene == 0 || splitScreen < TRANSITION_ANIMATION_TIME) {
		for (int i = 0; i < WATER_SIZE; ++i) {
			for (int j = 0; j < WATER_SIZE; ++j) {
				water.height(i, j) = (sin(time + water.vert(i,j)[0] * 10.8f +  water.vert(i,j)[2] * 5.7f) * 1.2f * cos(time - water.vert(i, j)[0] * 7.8f + water.vert(i, j)[2] * 13.7f) * sin(time + water.vert(i, j)[0] * 3.8f - water.vert(i, j)[2] * 9.7f) * cos(time - water.vert(i, j)[0] * 11.8f + water.vert(i, j)[2] * 6.7f));
				
			}
		}
		water.updateHeight();
	}
	if (scene == 2 || splitScreen < TRANSITION_ANIMATION_TIME) {
		for (int i = 0; i < MAX_SNOWFLAKES; ++i) {
			snowflakes[i].particle = snowflakes[i].particle + Vector3({ 0, float(-delta*70), 0 });
			if (snowflakes[i].particle[1] < -20.f) {
				snowflakes[i].particle = Vector3({ -cam.getPosition()[0] -150 + std::rand() % 50000 / 100.f,
					std::rand() % 30000 / 100.f,
					-cam.getPosition()[2] -150 + std::rand() % 50000 / 100.f });
			}

			snowflakes[i].buffer();

		}
		
	}



#pragma region FPS_CALCULATION
	{
		fps_Time_Acc += delta;
		counter++;
		if (fps_Time_Acc >= 1.f) {
			sprintf_s(charBuffer, 256, "FPS %d", counter);
			fps_Time_Acc = 0.f;
			counter = 0;
		}
	}
#pragma endregion
		
	r.hudText = std::string(charBuffer);
	
	
	if (splitScreen < TRANSITION_ANIMATION_TIME) {
		
		r.update(1-(splitScreen / TRANSITION_ANIMATION_TIME));
	}
	else
	r.update();



	Input::update();

	

}

void genLights(Vector4 colour, float dist, float startPos, int bufStart) {

	//Deffered Lights
	Vector3 sp[4] = { Vector3({ -startPos,40,startPos }),Vector3({ startPos,40,-startPos }),Vector3({ -startPos ,40,-startPos }),Vector3({ startPos,40,startPos }) };
	Vector3 offset[4] = { Vector3({ dist,0,0 }),Vector3({ -dist,0,0 }),Vector3({ 0,0,dist }),Vector3({ 0,0,-dist }) };
	for (int i = 0; i < 11; ++i) {
		lightArray[bufStart + i].pos = sp[0] + offset[0] * i;
		lightArray[bufStart + 11 + i].pos = sp[1] + offset[1] * i;
		lightArray[bufStart + 22 + i].pos = sp[2] + offset[2] * i;
		lightArray[bufStart + 33 + i].pos = sp[3] + offset[3] * i;
	}
	for (int i = 0; i < 44; ++i) {
		lightArray[bufStart + i].col = colour;
		lightArray[bufStart + i].rad = 130.f;
		lightsTwo.push_back(&(lightArray[bufStart + i]));
	}
}


int main(int argc, char** argv) {

	scenes[0].root = &h;
	scenes[0].lights = &lightsOne;
	scenes[1].root = &rootTwo;
	scenes[1].lights = &lightsTwo;
	scenes[2].root = &snowMap;
	scenes[2].lights = &lightsThree;
	
	genLights(Vector4({ 1, 0, 0, 1 }), 200, 1100, 0);
	genLights(Vector4({ 1, 1, 1, 1 }), 150, 825, 44);
	genLights(Vector4({ 0, 0, 1, 1 }), 100, 550, 88);



	//INIT OPENGL/FREEGLUT
	Initialize::init(argc, argv);

	r.init();
	
	//SHADER
	s = Shader("Shaders\\scene.vert", "Shaders\\scene.frag");
	shaderTerrain = Shader("Shaders\\heightmap.vert", "Shaders\\heightmap.frag");
	shaderReflect = Shader("Shaders/water.vert", "Shaders/water.frag");
	shaderParticle = Shader("Shaders/particle.vert", "Shaders/particle.frag", "Shaders/particle.geo");

	
	t = Texture("Textures/grass.jpg");
	rockTex = Texture("Textures/rock.jpg");

	snowTex = Texture("Textures/snow.jpg");

	snowflakeTex = Texture("Textures/snowflake.png");

	waterTex = Texture("Textures/water.jpg");

	iceTex = Texture("Textures/water.jpg","Textures/iceNormal.jpg");

	monkeyTex = Texture("Models/monkey.jpg");

	bumpMapTex = Texture("Textures/ASSdiffuse.jpg", "Textures/ASSnormals.jpg");


	for (int i = 0; i < MAX_SNOWFLAKES; ++i) {
		snowflakes.push_back(Particle(&snowflakeTex, &shaderParticle, 1.0f));
	}

	scenes[2].transparent = &(snowflakes[0]);


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



	///SCENE 3
	ice.init();
	snowMap.init();

	snowMap.readHM("Heightmap/map.jpg", 2048, 2048);
	snowMap.transform(Matrix4::scale(300, 0.3f, 300.f));
	snowMap.transform(Matrix4::translate(0, -50.f, 0));

	snowMap.addChild(&ice);

	ice.transform(Matrix4::scale(1.0f, 1.f, 1.0f));
	ice.transform(Matrix4::translate(0, 70.f, 0));

	lightsThree.push_back(&l);

	for (int i = 1; i < MAX_SNOWFLAKES; ++i) {
		snowflakes[0].addChild(&snowflakes[i]);
	}


	




	
	//r.addObject(&ice);
	
	

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



