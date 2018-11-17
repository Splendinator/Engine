#pragma once

#include <iostream>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "IO.h"
#include "Shader.h"
#include "Mesh.h"
#include "Object.h"
#include "Matrix4.h"
#include "Camera.h"
#include <vector>
#include <algorithm>
#include "SOIL\SOIL.h"
#include "HUD.h"

class Rasteriser
{
private:

	static const int REFLECTION_RESOLUTION = 1024;

	HUD h;

	//GLuint program;
	GLuint locModel;
	GLuint locView;
	GLuint locProj;

	//Perspective
	Matrix4 projection;
	//Matrix4 orthogonal; 

	//View
	Matrix4 view;

	
	//FBO - Frame Buffer Object.  
	GLuint FBObuffer;		//Holds data after initial scene render.
	GLuint FBOpostprocess;	//Holds data after postprocessing.
	GLuint FBOreflection;	//Used to calculate reflections. 

	//Textures for post processing.
	GLuint bufferDepthTex;		
	GLuint bufferColourTex[2];
	//GLuint bufferReflectionTex[6];
	GLuint bufferReflectColourTex;
	GLuint bufferReflectDepthTex;


	//Skybox
	GLuint cubeMapSkybox;

	Camera *camera;

	Mesh *quadMesh = Mesh::QuadInds();
	Texture texture;

	Shader pps;	//Post processing shader.
	Shader sbs; //Sky box shader.
	Shader shaderCopy;//Copy texture t into bound texture.


	Object quad = Object(quadMesh, &texture, &pps);


	std::vector<Object *> opaque;	//Opaque
	std::vector<Object *> objects;	//Transparent
	std::vector<Object *> reflectiveObjs;	//Reflective



	

	 
	void drawScene(Camera &c, const Matrix4 &proj);
	void drawSkyBox(Camera &c, const Matrix4 &proj);
	void postProcess();
	void presentScene();



public:

	const static int SCREEN_WIDTH = 1280;
	const static int SCREEN_HEIGHT = 720;

	Rasteriser();

	void update();

	~Rasteriser();

	void calculateReflections(Vector3 pos);

	void addObject(Object *o);
	void removeObject(Object *o);

	void setProjection(const Matrix4 &m);	

	void bindCamera(Camera *c);

	void init();
	void temp(Camera &c, const Matrix4 &proj);



};

inline void Rasteriser::bindCamera(Camera *c)
{
	camera = c;
}

inline void Rasteriser::addObject(Object * o)
{
	objects.push_back(o);
}

inline void Rasteriser::removeObject(Object * o)
{
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		if (*it == o) {
			objects.erase(it);
			break;
		}
	}
}

inline void Rasteriser::setProjection(const Matrix4 & m)
{
	projection = m;
}

