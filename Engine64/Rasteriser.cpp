#include "Rasteriser.h"




Rasteriser::Rasteriser()
{

}




void Rasteriser::update()
{
	glClear(GL_COLOR_BUFFER_BIT);

	

	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		glUniformMatrix4fv(locModel, 1, false, (GLfloat *)&(**it).transform);
		(**it).draw();
	}

	glutSwapBuffers();
}










Rasteriser::~Rasteriser()
{
	
}





