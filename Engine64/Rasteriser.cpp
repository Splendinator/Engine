#include "Rasteriser.h"




Rasteriser::Rasteriser()
{

}


int i;

void Rasteriser::update()
{
	Matrix4 model;
	Matrix4 view = camera->getTransform();

	glClear(GL_COLOR_BUFFER_BIT);
	
	

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) 
		(*it)-> calcCameraDist(camera);

	std::sort(objects.begin(), objects.end(), Object::compareCameraDistGT);

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		model = (**it).getTransform();
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "model"), 1, false, (GLfloat *)&model);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "view"), 1, false, (GLfloat *)&view);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID, "proj"), 1, false, (GLfloat *)&projection);
		glUniform1i(glGetUniformLocation((**it).s->programID, "theTexture"), 0);	//Sample from texture unit 0
		(*it)->draw();

	}



	glClear(GL_DEPTH_BUFFER_BIT);

	//glutSwapBuffers();
	glFlush();
}










Rasteriser::~Rasteriser()
{
	
}





