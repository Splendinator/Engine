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
	static const int SHADOW_RESOLUTION = 2048;

	HUD hud;


	std::string hudText = "FPS 60";

	

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
	GLuint FBOshadow; //Used to calculate shadowmap.

	GLuint FBOdefferedLight;
	GLuint FBOdefferedBuffer;



	 


	//Textures for post processing.
	GLuint bufferDepthTex;		
	GLuint bufferColourTex[2];
	GLuint bufferReflectionTex[6];
	GLuint bufferReflectColourTex;
	GLuint bufferReflectDepthTex;
	GLuint bufferShadowDepthTex;

	//Deffered
	GLuint bufferDeferredColour;
	GLuint bufferDeferredNormal;
	GLuint bufferDeferredDepth;
	GLuint bufferLightSpecular;
	GLuint bufferLightEmissive;

	
	//Cubemaps
	GLuint cubeMapSkybox;
	GLuint cubeMapReflection;
	GLuint cubeMapShadow; 

	Camera *camera;

	Mesh *quadMesh = Mesh::QuadInds();
	Mesh meshSphere = Mesh("Models/sphere.model");
	Texture texture;

	Shader pps;	//Post processing shader.
	Shader sbs; //Sky box shader.
	Shader shaderCopy;//Copy texture t into bound texture.
	Shader shaderText;//Draw text.
	Shader shaderShadow;//Real time shadows.

	Shader shaderScene;	//Fill GBuffers
	Shader shaderLight;	//Calculate Lighting
	Shader shaderCombine; //Combine previous two.


	Object quad = Object(quadMesh, &texture, &pps);
	Object sphere = Object(&meshSphere, &texture, &shaderLight);


	std::vector<Object *> opaque;	//Opaque
	std::vector<Object *> objects;	//Transparent
	std::vector<Object *> reflectiveObjs;	//Reflective



	 
	void drawScene(Camera &c, const Matrix4 &proj);
	void drawSkyBox(Camera &c, const Matrix4 &proj);
	void postProcess();
	void presentScene();
	void drawHUD();


	void deferredRender();

	void FillBuffers();
	void DrawPointLights();
	void CombineBuffers();



public:

	const static int SCREEN_WIDTH = 1280;
	const static int SCREEN_HEIGHT = 720;

	Rasteriser();

	void update();

	~Rasteriser();

	void calculateReflections(Vector3 pos);
	void calculateShadowmap(Vector3 pos);

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

