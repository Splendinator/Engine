#include "Rasteriser.h"




Rasteriser::Rasteriser()
{

}


int i;

void Rasteriser::update()
{
	Matrix4 MVP;
	Matrix4 VP = projection * camera->getTransform();
	glClear(GL_COLOR_BUFFER_BIT);
	
	

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		MVP = VP * (**it).getTransform();
		//glUniformMatrix4fv(10, 1, false, (GLfloat *) &MVP);	//TODO: Elegant way of doing this.
		//glUniform1i(11, (**it).t->id);
		glUniformMatrix4fv(glGetUniformLocation((**it).s->programID,"mvp"), 1, false, (GLfloat *)&MVP);
		glUniform1i(glGetUniformLocation((**it).s->programID, "theTexture"), 0);	//Sample from texture unit 0
		(**it).draw();
	}

	//std::cout << i++ << std::endl;

	//glutSwapBuffers();
	glFlush();
}










Rasteriser::~Rasteriser()
{
	
}





