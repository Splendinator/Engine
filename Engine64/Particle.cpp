#include "Particle.h"






void Particle::draw()
{

	glDisable(GL_CULL_FACE);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->id);




	glUniform1f(glGetUniformLocation(s->programID, "size"), size);
;

	glUniformMatrix4fv(glGetUniformLocation(s->programID, "model"), 1, false, (GLfloat *)&Matrix4::translate(particle[0], particle[1], particle[2]));


	glUseProgram(s->programID);
	glBindVertexArray(vao);

	
	glDrawArrays(GL_POINTS, 0, 1);

	


	glEnable(GL_CULL_FACE);

}

void Particle::buffer() {

	

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	//Verts
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3), &particle, GL_STATIC_DRAW);

	glVertexAttribPointer(VERTEX_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_ID);
	



	

}

Particle::~Particle()
{
}
