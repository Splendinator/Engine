#include "Rasteriser.h"




Rasteriser::Rasteriser()
{

}




void Rasteriser::update()
{
	Matrix4 MVP;
	Matrix4 VP = projection * camera->getTransform();
	glClear(GL_COLOR_BUFFER_BIT);
	
	

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		MVP = VP * (**it).transform;
		glUniformMatrix4fv(locMVP, 1, false, (GLfloat *)&MVP);
		(**it).draw();
	}
	
	glutSwapBuffers();
}










Rasteriser::~Rasteriser()
{
	
}





