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

Mesh::Mesh(std::string filepath)
{
	std::vector<Vector3> vvert;
	std::vector<Vector2> vtex;
	std::vector<Vector3> vnorm;
	std::vector<Vector3i> vinds;

	IO::readObj(filepath, vvert,vtex,vnorm, vinds);


 	



	num = vinds.size();

	verts = new Vector3[num];
	tex = new Vector2[num];
	alpha = new float[num];
	normals = new Vector3[num];

	for (int i = 0; i < num; ++i) {
		verts[i] = vvert[vinds[i][0]-1];
		tex[i] = vtex[vinds[i][1] - 1];
		normals[i] = vnorm[vinds[i][2] - 1];
		alpha[i] = 1.0f;
	}

	calculateTangents();

}


///For only vertices;
//Mesh::Mesh(std::string filepath)
//{
//	std::vector<Vector3> vverts;
//	std::vector<int> vinds;
//
//	IO::readObj(filepath, vverts, vinds);
//
//	num = vverts.size();
//
//	verts = new Vector3[num];
//	tex = new Vector2[num];
//	alpha = new float[num];
//	normals = new Vector3[num];
//
//	numIndicies = vinds.size();
//	inds = new GLuint[vinds.size()];
//
//	for (int i = 0; i < num; ++i) {
//		verts[i] = vverts[i];
//		tex[i] = Vector2({0,0});
//		alpha[i] = 1.0f;
//		normals[i] = Vector3({ 0,0,0 });
//	}
//
//	for (int i = 0; i < numIndicies; ++i) {
//		inds[i] = vinds[i] - 1;
//	}
//
//	calculateNormals();
//
//
//}


void Mesh::calculateNormals()
{	

	if (!inds) {
		std::cout << "Calculating normals with no indicies, TODO: add this." << std::endl;
		return;
	}

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
		//std::cout << normals[i] << std::endl;
	}
	

}





inline Vector3 Mesh::GenerateTangent(const Vector3 & a, const Vector3 & b, const Vector3 & c, const Vector2 & ta, const Vector2 & tb, const Vector2 & tc)
{
		Vector2 coord1 = tb - ta;
		Vector2 coord2 = tc - ta;

		Vector3 vertex1 = b - a;
		Vector3 vertex2 = c - a;

		Vector3 axis = Vector3(vertex1 * coord2[1] - vertex2 * coord1[1]);

		float factor = 1.0f / (coord1[0] * coord2[1] - coord2[0] * coord1[1]);

		return axis * factor;

}


void Mesh::calculateTangents()
{
	if (!tangents) tangents = new Vector3[num];
	if (!tex) return;

	memset(tangents, 0, sizeof(Vector3) * num);


	if (inds) {
		for (int i = 0; i < numIndicies; i += 3) {
			int a = inds[i];
			int b = inds[i + 1];
			int c = inds[i + 2];


			Vector3 v = GenerateTangent(verts[a], verts[b], verts[c], tex[a], tex[b], tex[c]);

			tangents[a] += v;
			tangents[b] += v;
			tangents[c] += v;

		}
	}
	else
	{
		for (GLuint i = 0; i < num; i += 3) {
			Vector3 v = GenerateTangent(verts[i], verts[i + 1], verts[i + 2], tex[i], tex[i + 1], tex[i + 2]);

			tangents[i] += v;
			tangents[i + 1] += v;
			tangents[i + 2] += v;
		}
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

		glVertexAttribPointer(NORMAL_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_ID);
	}


	//Tangents
	if (tangents) {
		glGenBuffers(1, &tangentId);
		glBindBuffer(GL_ARRAY_BUFFER, tangentId); 
		glBufferData(GL_ARRAY_BUFFER, num * sizeof(Vector3), tangents, GL_STATIC_DRAW);

		glVertexAttribPointer(TANGENT_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TANGENT_ID);
	}


	//Normals
	if (binormals) {
		glGenBuffers(1, &binormalId);
		glBindBuffer(GL_ARRAY_BUFFER, binormalId);
		glBufferData(GL_ARRAY_BUFFER, num * sizeof(Vector3), binormals, GL_STATIC_DRAW);

		glVertexAttribPointer(BINORMAL_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(BINORMAL_ID);
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




Mesh *Mesh::QuadInds()
{
	Mesh *m = new Mesh;
	m->num = 4;
	m->verts = new Vector3[m->num]{ Vector3({-1,-1,0}), Vector3({-1,1,0}), Vector3({ 1,1,0}), Vector3({ 1,-1,0}) };
	m->tex = new Vector2[m->num]{ Vector2({0,0}),  Vector2({0,1}),  Vector2({1,1}),  Vector2({1,0}) };
	m->alpha = new GLfloat[4]{ 1,1,1,1 };
	m->normals = new Vector3[4]{ Vector3({ 0,0,1 }),Vector3({ 0,0,1 }),Vector3({ 0,0,1 }),Vector3({ 0,0,1 }) };

	m->numIndicies = 6;
	m->inds = new GLuint[6]{ 1,0,2,2,0,3 };

	m->calculateTangents();

	return m;
}



Mesh *Mesh::Point()
{
	Mesh *m = new Mesh;

	m->num = 1;
	m->verts = new Vector3[m->num]{ Vector3({ 0,0,0 })};
	m->alpha = new GLfloat[m->num]{ 1 };
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

			//normals
			m->normals[(z*zNum + x)] = Vector3({ 0,1,0 });

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
	m->calculateTangents();
	return m;
}
 
