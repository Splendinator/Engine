#include "Rasteriser.h"




Rasteriser::Rasteriser()
{

}

void Rasteriser::drawScene() {
	Matrix4 model;
	view = camera->getTransform();

	glBindFramebuffer(GL_FRAMEBUFFER, FBObuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	


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
	//glDisable(GL_DEPTH);
	glBindFramebuffer(GL_FRAMEBUFFER,FBOpostprocess);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(pps.programID);

	glUniform2f(glGetUniformLocation(pps.programID,
		" pixelSize "), 1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT);
	texture.id = bufferColourTex[0];

	quad.draw();


	//glEnable(GL_DEPTH);


}

void Rasteriser::presentScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	quad.t->id = bufferColourTex[1];
	quad.draw();
}

void Rasteriser::update()
{

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
	0 , GL_DEPTH_STENCIL , GL_UNSIGNED_INT_24_8 , NULL );	//COLOUR TEXTURES
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
	GL_TEXTURE_2D , bufferColourTex [0] , 0);		glGenFramebuffers(1, &FBOpostprocess);	pps = Shader("Shaders\\pp.vert","Shaders\\pp.frag");	quad.init();
}





Rasteriser::~Rasteriser()
{
	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &FBObuffer);
	glDeleteFramebuffers(1, &FBOpostprocess);
}





