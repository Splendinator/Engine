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

class Rasteriser
{
private:


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

	//Textures for post processing.
	GLuint bufferDepthTex;		
	GLuint bufferColourTex[2];


	//Skybox
	GLuint cubeMapSkybox;

	Camera *camera;

	Mesh *quadMesh = Mesh::QuadInds();
	Texture texture;

	Shader pps;	//Post processing shader.
	Shader sbs; //Sky box shader.


	Object quad = Object(quadMesh, &texture, &pps);



	std::vector<Object *> objects;	//Vector of all the objects will be drawn


	

	 
	void drawScene();
	void drawSkyBox();
	void postProcess();
	void presentScene();



public:

	const static int SCREEN_WIDTH = 1280;
	const static int SCREEN_HEIGHT = 720;

	Rasteriser();

	void update();

	~Rasteriser();

	void addObject(Object *o);
	void removeObject(Object *o);

	void setProjection(const Matrix4 &m);	

	void bindCamera(Camera *c);

	void init();



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

