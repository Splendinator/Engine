		#include "Rasteriser.h"

 


Rasteriser::Rasteriser()
{

}

void Rasteriser::drawScene(Camera &c, const Matrix4 &proj) {
	Matrix4 model;
	Matrix4 view = c.getTransform();
	Matrix4 shadowMat = Matrix4::Bias() * Matrix4::Perspective(0.01f, 10, PI / 2.f, 1.f);

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
		(*it)->calcCameraDist(&c);

	std::sort(objects.begin(), objects.end(), Object::compareCameraDistGT);

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {

		model = (**it).getTransform();

		glUseProgram((*it)->s->programID);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "model"), 1, false, (GLfloat *)&model);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "view"), 1, false, (GLfloat *)&view);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "proj"), 1, false, (GLfloat *)&proj);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "shadowMat"), 1, false, (GLfloat *)&shadowMat);
		glUniform1i(glGetUniformLocation((**it).s->programID, "theTexture"), 0);
		glUniform1i(glGetUniformLocation((**it).s->programID, "shadowTex"), 1);		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bufferShadowDepthTex);		 
		(*it)->draw();

	}

}
void Rasteriser::postProcess() {
	
	glBindFramebuffer(GL_FRAMEBUFFER,FBOpostprocess);
	glUseProgram(pps.programID);
	glUniform2f(glGetUniformLocation(pps.programID,
		"pixelSize"), 1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_COLOR_BUFFER_BIT);
	texture.id = bufferColourTex[0];
	quad.draw();


	///Multiple Passes
	//for (int i = 0; i < 600; ++i) {
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	texture.id = bufferColourTex[0];
	//	quad.draw();
	//
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	texture.id = bufferColourTex[1];
	//	quad.draw();
	//}




}

void Rasteriser::presentScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	quad.t->id = bufferColourTex[1];
	quad.draw();
}

void Rasteriser::drawSkyBox(Camera &c, const Matrix4 &proj) {


	glDisable(GL_DEPTH_TEST);

	glUseProgram(sbs.programID);

	Matrix4 view = c.getTransform();


	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapSkybox);

	glUniformMatrix4fv(glGetUniformLocation(sbs.programID, "proj"), 1, false, (GLfloat *)&proj);
	glUniformMatrix4fv(glGetUniformLocation(sbs.programID, "view"), 1, false, (GLfloat *)&view);
	glUniform1i(glGetUniformLocation(sbs.programID, "cubeTex"), 0);


	quad.draw();

	glEnable(GL_DEPTH_TEST);

}


void Rasteriser::drawHUD() {

	Matrix4 orth = Matrix4::Orthogonal(0, SCREEN_WIDTH, -1, 1, SCREEN_HEIGHT,0);
	Matrix4 view = Matrix4::identity();
	Matrix4 model;



	glDisable(GL_DEPTH_TEST);
	
	glUseProgram(shaderText.programID);
	

	glUniformMatrix4fv(glGetUniformLocation(shaderText.programID, "proj"), 1, false, (GLfloat *)&orth);
	glUniformMatrix4fv(glGetUniformLocation(shaderText.programID, "view"), 1, false, (GLfloat *)&view);


	for (int i = 0; i < hudText.length(); ++i) {
		model = Matrix4::identity() * Matrix4::translate(-SCREEN_WIDTH + 32*( i+1), -16, 0) * Matrix4::scale(16.f, 16.f, 2.f);
		glUniformMatrix4fv(glGetUniformLocation(shaderText.programID, "model"), 1, false, (GLfloat *)&model);
		texture.id = hud.textures[hudText[i]];
		quad.draw();
	}

	glEnable(GL_DEPTH_TEST);
}

void Rasteriser::deferredRender()
{
	FillBuffers();
	DrawPointLights();
	CombineBuffers();
}


//Fills Colour, Normals, and Depth buffers.
void Rasteriser::FillBuffers()
{

	Matrix4 view = camera->getTransform();
	Matrix4 model;

	glBindFramebuffer(GL_FRAMEBUFFER, FBOdefferedBuffer);	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	glUseProgram(shaderScene.programID);	glUniform1i(glGetUniformLocation(shaderScene.programID, "texDiffuse"), 0);	glUniform1i(glGetUniformLocation(shaderScene.programID, "texNorms"), 1);	glUniformMatrix4fv(glGetUniformLocation(shaderText.programID, "proj"), 1, false, (GLfloat *)&projection);
	glUniformMatrix4fv(glGetUniformLocation(shaderText.programID, "view"), 1, false, (GLfloat *)&view);	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {

		model = (**it).getTransform();

		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "model"), 1, false, (GLfloat *)&model);
		(*it)->draw();

	}
}

void Rasteriser::DrawPointLights()
{
	
	
	Matrix4 view = camera->getTransform();
	Matrix4 model;

	

	glBindFramebuffer(GL_FRAMEBUFFER, FBOdefferedLight);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_ONE, GL_ONE);

	glUseProgram(shaderLight.programID);

	glUniform1i(glGetUniformLocation(shaderLight.programID, "texDepth"), 3);	glUniform1i(glGetUniformLocation(shaderLight.programID, "texNorms"), 4);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bufferDeferredDepth);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bufferDeferredNormal);

	glUniform3fv(glGetUniformLocation(shaderLight.programID, "cameraPos"),1,(GLfloat *)&camera->getPosition());
	glUniform2f(glGetUniformLocation(shaderLight.programID, "pixelSize"), 1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT);


	glUniform1f(glGetUniformLocation(shaderLight.programID, "lightRadius"), 30.f);
	glUniform3fv(glGetUniformLocation(shaderLight.programID, "lightPos"),1, (GLfloat *)&Vector3({ 0,0,0 }));
	glUniform4fv(glGetUniformLocation(shaderLight.programID, "lightColour"), 1, (GLfloat *)&Vector4({ 0.2f,0.8f,0.2f,1.0f }));


	//sphere.transform();
	model = Matrix4::scale(30.f, 30.f, 30.f);

	float dist = (camera->getPosition()).length();
	if (dist < 30.f)
		glCullFace(GL_FRONT);
	else
		glCullFace(GL_BACK);




	glUniformMatrix4fv(glGetUniformLocation(shaderLight.programID, "proj"), 1, false, (GLfloat *)&projection);
	glUniformMatrix4fv(glGetUniformLocation(shaderLight.programID, "view"), 1, false, (GLfloat *)&view);
	glUniformMatrix4fv(glGetUniformLocation(shaderLight.programID, "model"), 1, false, (GLfloat *)&model);


	
	sphere.draw();
		glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glClearColor(0.2f, 0.2f, 0.2f, 1);	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Rasteriser::CombineBuffers()
{
	glDisable(GL_DEPTH_TEST);
	glUseProgram(shaderCombine.programID);

	glUniform1i(glGetUniformLocation(shaderCombine.programID, "texDiffuse"), 2);
	glUniform1i(glGetUniformLocation(shaderCombine.programID, "texSpecular"), 3);
	glUniform1i(glGetUniformLocation(shaderCombine.programID, "texEmissive"), 4);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, bufferDeferredColour);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bufferLightEmissive);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bufferLightSpecular);

	
	quad.draw();
	
	glEnable(GL_DEPTH_TEST);
}





void Rasteriser::update()
{
	deferredRender();
	//calculateReflections(Vector3({ 0,0,0 }));
	//calculateShadowmap(Vector3({ 0,0,0 }));

	//glBindFramebuffer(GL_FRAMEBUFFER, FBObuffer);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	
	//drawSkyBox(*camera,projection);
	//drawScene(*camera,projection);
	//postProcess();
	//presentScene();

	//drawHUD();
	
	//glutSwapBuffers();
	glFlush();
}









Rasteriser::~Rasteriser()
{
	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &FBObuffer);
	glDeleteFramebuffers(1, &FBOpostprocess);
}


void Rasteriser::calculateReflections(Vector3 pos) {
	//Matrix4 proj = Matrix4::Perspective(0.1f, 1000.f, PI/2, 1.f);

	Matrix4 proj = Matrix4::Perspective(0.01f, 1000, PI / 2.f, 1.f);


	Camera c[6];
	
	for (int i = 0; i < 6; ++i) c[i].move(pos);

	c[1].rollYaw(-PI / 2);
	c[0].rollYaw(PI / 2);
	c[2].rollPitch(PI / 2);
	c[3].rollPitch(-PI / 2);
	c[5].rollYaw(PI);
	//c[5].rollYaw(-PI/2);


	/*
	0	GL_TEXTURE_CUBE_MAP_POSITIVE_X
	1	GL_TEXTURE_CUBE_MAP_NEGATIVE_X
	2	GL_TEXTURE_CUBE_MAP_POSITIVE_Y
	3	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
	4	GL_TEXTURE_CUBE_MAP_POSITIVE_Z
	5	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	*/

	
	glViewport(0, 0, REFLECTION_RESOLUTION, REFLECTION_RESOLUTION);


	for (int i = 0; i < 6; ++i) {

		glBindFramebuffer(GL_FRAMEBUFFER, FBOreflection);
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//drawSkyBox(c[i], proj);
		drawScene(c[i], proj);

		glBindFramebuffer(GL_FRAMEBUFFER, FBOpostprocess);

		glUseProgram(shaderCopy.programID);
		

		//TODO: Don't need to copy texture twice.
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferReflectionTex[i], 0);
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeMapReflection, 0, i);
		glClear(GL_COLOR_BUFFER_BIT);
		texture.id = bufferReflectColourTex;


		quad.draw();
	}
	
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	
}

void Rasteriser::calculateShadowmap(Vector3 pos) {

	//std::cout << glGetError() << std::endl;
	Matrix4 proj = Matrix4::Perspective(0.01f, 10, PI / 2.f, 1.f);
	Camera c;
	c.move(pos);

	glBindFramebuffer(GL_FRAMEBUFFER, FBOshadow);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);	glUseProgram(shaderShadow.programID);


	drawSkyBox(c, proj);
	drawScene(c, proj);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);





}






void Rasteriser::init()
{


	//DEPTH TEXTURE
	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);


	//COLOUR TEXTURES
	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	for (int i = 0; i < 6; ++i) {
		glGenTextures(1, &bufferReflectionTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferReflectionTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, REFLECTION_RESOLUTION, REFLECTION_RESOLUTION, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);


	}




	//Reflection Textures
	glGenTextures(1, &bufferReflectDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferReflectDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, REFLECTION_RESOLUTION, REFLECTION_RESOLUTION,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glGenTextures(1, &bufferReflectColourTex);
	glBindTexture(GL_TEXTURE_2D, bufferReflectColourTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, REFLECTION_RESOLUTION, REFLECTION_RESOLUTION, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);





	//Shadow map
	glGenTextures(1, &bufferShadowDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferShadowDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_RESOLUTION, SHADOW_RESOLUTION,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);




	//GLuint bufferDefferedColour;
	glGenTextures(1, &bufferDeferredColour);
	glBindTexture(GL_TEXTURE_2D,bufferDeferredColour);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);


	//GLuint bufferDefferedNormal;
	glGenTextures(1, &bufferDeferredNormal);
	glBindTexture(GL_TEXTURE_2D, bufferDeferredNormal);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//GLuint bufferDefferedDepth;
	glGenTextures(1, &bufferDeferredDepth);
	glBindTexture(GL_TEXTURE_2D, bufferDeferredDepth);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	//GLuint bufferLightSpecular;
	glGenTextures(1, &bufferLightSpecular);
	glBindTexture(GL_TEXTURE_2D, bufferLightSpecular);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//GLuint bufferLightEmissive;
	glGenTextures(1, &bufferLightEmissive);
	glBindTexture(GL_TEXTURE_2D, bufferLightEmissive);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	









	//FRAME BUFFER
	glGenFramebuffers(1, &FBObuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FBObuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex[0], 0);

	glGenFramebuffers(1, &FBOpostprocess);


	//Reflection FBO
	glGenFramebuffers(1, &FBOreflection);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOreflection);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferReflectDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, bufferReflectDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferReflectColourTex, 0);


	//Shadow FBO
	glGenFramebuffers(1, &FBOshadow);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOshadow);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferShadowDepthTex, 0);

	glGenFramebuffers(1, &FBOdefferedLight);
	glGenFramebuffers(1, &FBOdefferedBuffer);





	//Deferred Rendering
	GLenum texBuffers[2] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1
	};


	glBindFramebuffer(GL_FRAMEBUFFER, FBOdefferedBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	GL_TEXTURE_2D, bufferDeferredColour, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
	GL_TEXTURE_2D, bufferDeferredNormal, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	GL_TEXTURE_2D, bufferDeferredDepth, 0);
	glDrawBuffers(2, texBuffers);	std::cout << FBOdefferedBuffer << std::endl;
	std::cout << bufferDeferredColour << " " << bufferDeferredNormal << " " << bufferDeferredDepth << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, FBOdefferedLight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferLightEmissive, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, bufferLightSpecular, 0);;
	glDrawBuffers(2, texBuffers);


	std::cout << FBOdefferedLight << std::endl;
	std::cout << bufferLightEmissive << " " << bufferLightSpecular << std::endl;





	pps = Shader("Shaders\\pp.vert", "Shaders\\pp.frag");
	sbs = Shader("Shaders\\skybox.vert", "Shaders\\skybox.frag");
	shaderCopy = Shader("Shaders\\copy.vert", "Shaders\\copy.frag");
	shaderText = Shader("Shaders/text.vert", "Shaders/text.frag");
	shaderShadow = Shader("Shaders/shadow.vert", "Shaders/shadow.frag");
	shaderScene = Shader("Shaders/scene.vert","Shaders/scene.frag");
	shaderLight	= Shader("Shaders/light.vert","Shaders/light.frag");
	shaderCombine = Shader("Shaders/combine.vert","Shaders/combine.frag");
					
	quad.init();
	sphere.init();

	cubeMapSkybox = SOIL_load_OGL_cubemap(
		"Textures/interstellar_+x.png",
		"Textures/interstellar_-x.png",
		"Textures/interstellar_+y.png",
		"Textures/interstellar_-y.png",
		"Textures/interstellar_+z.png",
		"Textures/interstellar_-z.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &cubeMapReflection);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapReflection);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &cubeMapShadow);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapShadow);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);




	hud.init();

}
