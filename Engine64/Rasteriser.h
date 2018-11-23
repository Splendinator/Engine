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
#include "PointLight.h"

class Rasteriser
{
private:

	static const int REFLECTION_RESOLUTION = 1024;
	static const int SHADOW_RESOLUTION = 2048;

	static const int POST_PROCESSING_EFFECTS = 4;

	HUD hud;


	int ppRepeat[POST_PROCESSING_EFFECTS] = { 0,2,4,1 };

	

	//GLuint program;
	GLuint locModel;
	GLuint locView;
	GLuint locProj;

	//Perspective
	Matrix4 projection;
	//Matrix4 orthogonal; 

	//View
	Matrix4 view;


	//Shadow
	Matrix4 matShadow;
	Matrix4 prevMatShadow;


	
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



	//Split screen
	GLuint FBOSplitScreen;
	GLuint firstHalf;
	


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

	Shader shaderMerge; //Merge two screens for splitscreen.


	Object quad = Object(quadMesh, &texture, &pps);
	Object sphere = Object(&meshSphere, &texture, &shaderLight);


	std::vector<Object *> transparent;	//Opaque
	std::vector<Object *> objects;	//Transparent
	std::vector<Object *> reflectiveObjs;	//Reflective
	std::vector<PointLight *> pointLights;	//Reflective


	std::vector<Object *> prevObjects;
	std::vector<PointLight *> prevPointLights;
	std::vector<Object *> prevTransparent;



	 
	void drawScene(Camera &c, const Matrix4 &proj);
	void drawSkyBox(Camera &c, const Matrix4 &proj);
	void postProcess();
	void presentScene(GLuint buffer);
	void drawHUD();
	void presentSplitScreen(float pct);


	void deferredRender(Camera &c, const Matrix4 &proj);

	void FillBuffers(Camera &c, const Matrix4 &proj);
	void DrawPointLights(Camera &c, const Matrix4 &proj);
	void CombineBuffers(Camera &c, const Matrix4 &proj);



public:

	int postProcessEffect = 0;


	std::string hudText = "";

	const static int SCREEN_WIDTH = 1920;
	const static int SCREEN_HEIGHT = 1200;

	Rasteriser();

	void update();

	void update(float pct);

	~Rasteriser();

	GLuint calculateReflections(Vector3 pos);
	void calculateShadowmap(Vector3 pos, float yaw);
	void calculateShadowmaps(Vector3 pos);

	void addObject(Object *o);
	void removeObject(Object *o);

	void addTransparent(Object *o);

	void addPointLight(PointLight *p);

	void setProjection(const Matrix4 &m);	

	void bindCamera(Camera *c);

	void init();

	void clear();


};

inline void Rasteriser::bindCamera(Camera *c)
{
	camera = c;
}

inline void Rasteriser::addObject(Object * o)
{
	objects.push_back(o);
	for (std::vector<Object *>::iterator it = o->children.begin(); it != o->children.end(); ++it) {
		addObject(*it);
	}
}


inline void Rasteriser::addTransparent(Object * o)
{
	transparent.push_back(o);
	for (std::vector<Object *>::iterator it = o->children.begin(); it != o->children.end(); ++it) {
		addTransparent(*it);
	}
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

inline void Rasteriser::addPointLight(PointLight *p)
{
	pointLights.push_back(p);
}

inline void Rasteriser::setProjection(const Matrix4 & m)
{
	projection = m;
}

