#include "Rasteriser.h"




Rasteriser::Rasteriser()
{

}

void Rasteriser::drawScene() {
	Matrix4 model;
	


	


	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
		(*it)->calcCameraDist(camera);

	std::sort(objects.begin(), objects.end(), Object::compareCameraDistGT);

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		
		model = (**it).getTransform();
		
		glUseProgram((*it)->s->programID);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "model"), 1, false, (GLfloat *)&model);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "view"), 1, false, (GLfloat *)&view);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "proj"), 1, false, (GLfloat *)&projection);
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

void Rasteriser::drawSkyBox() {

	
	glDisable(GL_DEPTH_TEST);

	glUseProgram(sbs.programID);

	glUniformMatrix4fv(glGetUniformLocation(sbs.programID, "proj"), 1, false, (GLfloat *)&projection);
	glUniformMatrix4fv(glGetUniformLocation(sbs.programID, "view"), 1, false, (GLfloat *)&view);
	//glBindTexture(GL_TEXTURE_3D, cubeMapSkybox);

	quad.draw();

	//glClear(GL_DEPTH_BUFFER);
	glEnable(GL_DEPTH_TEST);

}

void Rasteriser::update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBObuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = camera->getTransform();
	drawSkyBox();
	drawScene();
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



	pps = Shader("Shaders\\pp.vert","Shaders\\pp.frag");
	sbs = Shader("Shaders\\skybox.vert", "Shaders\\skybox.frag");

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





