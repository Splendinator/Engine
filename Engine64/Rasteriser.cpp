#include "Rasteriser.h"




Rasteriser::Rasteriser()
{

}

void Rasteriser::drawScene(Camera &c, const Matrix4 &proj) {
	Matrix4 model;
	Matrix4 view = c.getTransform();


	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
		(*it)->calcCameraDist(&c);

	std::sort(objects.begin(), objects.end(), Object::compareCameraDistGT);

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {

		model = (**it).getTransform();

		glUseProgram((*it)->s->programID);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "model"), 1, false, (GLfloat *)&model);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "view"), 1, false, (GLfloat *)&view);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "proj"), 1, false, (GLfloat *)&proj);
		glUniform1i(glGetUniformLocation((**it).s->programID, "theTexture"), 0);	//Sample from texture unit 0
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


std::string s = "FPS";

void Rasteriser::update()
{

	calculateReflections(Vector3({ 0,0,0 }));

	glBindFramebuffer(GL_FRAMEBUFFER, FBObuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	
	drawSkyBox(*camera,projection);
	drawScene(*camera,projection);
	postProcess();
	presentScene();
	
	//glutSwapBuffers();
	glFlush();
}




void Rasteriser::init()
{
	//DEPTH TEXTURE
	glGenTextures (1 , &bufferDepthTex );
	glBindTexture ( GL_TEXTURE_2D , bufferDepthTex );
	glTexParameterf ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP );
	glTexParameterf ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP );
	glTexParameterf ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST );
	glTexParameterf ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST );
	glTexImage2D ( GL_TEXTURE_2D , 0 , GL_DEPTH24_STENCIL8 , SCREEN_WIDTH , SCREEN_HEIGHT,
	0 , GL_DEPTH_STENCIL , GL_UNSIGNED_INT_24_8 , NULL );


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


	//FRAME BUFFER
	glGenFramebuffers(1, &FBObuffer);
	glBindFramebuffer ( GL_FRAMEBUFFER , FBObuffer );
	glFramebufferTexture2D ( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT ,
	GL_TEXTURE_2D , bufferDepthTex , 0);
	glFramebufferTexture2D ( GL_FRAMEBUFFER , GL_STENCIL_ATTACHMENT ,
	GL_TEXTURE_2D , bufferDepthTex , 0);
	glFramebufferTexture2D ( GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 ,
	GL_TEXTURE_2D , bufferColourTex [0] , 0);
	
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



	pps = Shader("Shaders\\pp.vert","Shaders\\pp.frag");
	sbs = Shader("Shaders\\skybox.vert", "Shaders\\skybox.frag");
	shaderCopy = Shader("Shaders\\copy.vert", "Shaders\\copy.frag");

	quad.init();

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


}





Rasteriser::~Rasteriser()
{
	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &FBObuffer);
	glDeleteFramebuffers(1, &FBOpostprocess);
}


void Rasteriser::calculateReflections(Vector3 pos) {
	Matrix4 proj = Matrix4::Perspective(0.1f, 1000.f, PI/2, 1.f);

	Camera c[6];
	
	for (int i = 0; i < 6; ++i) c[i].move(pos);

	c[0].rollYaw(PI / 2);
	c[1].rollYaw(-PI / 2);
	c[2].rollPitch(PI / 2);
	c[3].rollPitch(-PI / 2);
	c[4].rollYaw(PI);
	c[5].rollYaw(-PI/2);


	/*
	0	GL_TEXTURE_CUBE_MAP_POSITIVE_X
	1	GL_TEXTURE_CUBE_MAP_NEGATIVE_X
	2	GL_TEXTURE_CUBE_MAP_POSITIVE_Y
	3	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
	4	GL_TEXTURE_CUBE_MAP_POSITIVE_Z
	5	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	*/

	



	for (int i = 0; i < 6; ++i) {

		glBindFramebuffer(GL_FRAMEBUFFER, FBOreflection);
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		drawSkyBox(c[i], proj);
		drawScene(c[i], proj);

		glBindFramebuffer(GL_FRAMEBUFFER, FBOpostprocess);
		glUseProgram(shaderCopy.programID);


		//TODO: Don't need to copy texture twice.
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferReflectionTex[i], 0);
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeMapSkybox, 0, 2);
		glClear(GL_COLOR_BUFFER_BIT);
		texture.id = bufferReflectColourTex;


		quad.draw();
	}
	



	
}





