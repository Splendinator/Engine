#include "Mesh.h"



Mesh::Mesh()
{
	verts = nullptr;
	tex = nullptr;
	alpha = nullptr;
	inds = nullptr;
}


Mesh::~Mesh()
{
	delete [] verts;
	delete [] tex;
	delete [] alpha;
	delete [] inds;
}


void Mesh::calculateNormals()
{	
	memset(normals, 0, sizeof(Vector3) * num);

	for (int i = 0; i < numIndicies; i+=3) {
		int a = inds[i];
		int b = inds[i + 1];
		int c = inds[i + 2];

		//std::cout << verts[b] - verts[a] << " " << verts[c] - verts[a] << " " << Util::cross(verts[b] - verts[a], verts[c] - verts[a]) << std::endl;

		Vector3 v = Util::cross(verts[b] - verts[a], verts[c] - verts[a]);
	

		normals[a] += v;
		normals[b] += v;
		normals[c] += v;

	}

	for (int i = 0; i < num; ++i) {
		normals[i].normalise();
	}

}

void Mesh::buffer(){
	
	//VAO
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);


	//Verts
	glGenBuffers(1, &vertId);
	glBindBuffer(GL_ARRAY_BUFFER, vertId);
	glBufferData(GL_ARRAY_BUFFER, num * sizeof(Vector3), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(VERTEX_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_ID);


	//Tex
	glGenBuffers(1, &texId);
	glBindBuffer(GL_ARRAY_BUFFER, texId);
	glBufferData(GL_ARRAY_BUFFER, num * sizeof(Vector2), tex, GL_STATIC_DRAW);

	glVertexAttribPointer(TEXTURE_ID, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(TEXTURE_ID);
	

	//Alpha
	if (alpha) {
		glGenBuffers(1, &alphaId);
		glBindBuffer(GL_ARRAY_BUFFER, alphaId);
		glBufferData(GL_ARRAY_BUFFER, num * sizeof(GLfloat), alpha, GL_STATIC_DRAW);

		glVertexAttribPointer(ALPHA_ID, 1, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(ALPHA_ID);
	}

	//Normals
	if (normals) {
		glGenBuffers(1, &normalId);
		glBindBuffer(GL_ARRAY_BUFFER, normalId);
		glBufferData(GL_ARRAY_BUFFER, num * sizeof(Vector3), normals, GL_STATIC_DRAW);

		glVertexAttribPointer(NORMALS_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMALS_ID);
	}


	//Indicies
	if (inds) {
	
		glGenBuffers(1, &indsId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indsId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndicies * sizeof(GLuint), inds, GL_STATIC_DRAW);


		//Don't need this part since it is assumed indicies are unsigned ints of size 1
		///glVertexAttribPointer(INDICIES_ID, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		///glEnableVertexAttribArray(INDICIES_ID);
	}

}

//Mesh *Mesh::Triangle() {
//	Mesh *m = new Mesh;
//	m->num = 3;
//	m->verts = new Vector3[m->num]{ -1,-1,0,0,1,0,1,-1,0 };
//	m->tex = new GLfloat[6]{ 0,0,0.5,1,1,0 };
//	m->alpha = new GLfloat[3]{ 1,1,1 };
//	return m;
//}

//Mesh *Mesh::Quad()
//{
//	Mesh *m = new Mesh;
//	m->num = 6;
//	m->verts = new GLfloat[18]{ -1,-1,0,-1,1,0,1,-1,0 , -1 ,1 ,0 , 1 ,1,0, 1,-1,0  };
//	m->tex = new GLfloat[12]{ 0,0,0,1,1,0,0,1,1,1,1,0 };
//	m->alpha = new GLfloat[6]{ 0.1f,0.1f,0.1f,0.1f,0.1f,0.1f };
//	return m;
//}

Mesh *Mesh::QuadInds()
{
	Mesh *m = new Mesh;
	m->num = 4;
	m->verts = new Vector3[m->num]{ Vector3({-1,-1,0}), Vector3({-1,1,0}), Vector3({ 1,1,0}), Vector3({ 1,-1,0}) };
	m->tex = new Vector2[m->num]{ Vector2({0,0}),  Vector2({0,1}),  Vector2({1,1}),  Vector2({1,0}) };
	m->alpha = new GLfloat[4]{ 0.1f,0.1f,0.1f,0.1f };
	m->normals = new Vector3[m->num];

	m->numIndicies = 6;
	m->inds = new GLuint[6]{ 1,0,2,2,0,3 };

	return m;
}

Mesh *Mesh::Plane(int xNum, int zNum, float xTexture, float zTexture) {
	Mesh *m = new Mesh;
	m->num = xNum * zNum;
	m->numIndicies = 6 * (xNum-1) * (zNum-1);

	m->verts = new Vector3[m->num];
	m->tex = new Vector2[m->num];
	m->alpha = new GLfloat[m->num];
	m->inds = new GLuint[m->numIndicies];
	m->normals = new Vector3[m->num];

	float xNumf = xNum-1;
	float zNumf = zNum-1;

	//std::cout << m->num << " " << m->numIndicies;

	for (int z = 0; z < zNum; ++z){
		for (int x = 0; x < xNum; ++x){
			
			//verts
			m->verts[(z*zNum + x)][0] = (2 * x / xNumf) - 1;
			m->verts[(z*zNum + x)][1] = 0.f;
			m->verts[(z*zNum + x)][2] = (2 * z / zNumf) - 1;

			//texCoords
			m->tex[(z*zNum + x)][0] = x / xNumf * xTexture;
			m->tex[(z*zNum + x)][1] = z / zNumf * zTexture;

			//alpha
			m->alpha[(z*zNum + x) + 0] = 1.0f;

		}
	}


	for (int z = 0; z < zNum-1; ++z) {
		for (int x = 0; x < xNum-1; ++x) {

			//inds
			m->inds[6 * (z*(zNum - 1) + x) + 0] = (z*zNum + x);
			m->inds[6 * (z*(zNum - 1) + x) + 1] = ((z+1)*zNum + x);
			m->inds[6 * (z*(zNum - 1) + x) + 2] = ((z + 1)*zNum + (x+1));
											
			m->inds[6 * (z*(zNum - 1) + x) + 3] = (z*zNum + x);
			m->inds[6 * (z*(zNum - 1) + x) + 4] = ((z + 1)*zNum + (x + 1));
			m->inds[6 * (z*(zNum - 1) + x) + 5] = (z*zNum + (x+1));

		}
	}

	return m;
}
 
