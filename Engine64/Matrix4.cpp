#include "Matrix4.h"



Matrix4::Matrix4()
{
}


Matrix4::~Matrix4()
{
}

Matrix4 Matrix4::operator*(Matrix4 rhs)
{
	Matrix4 ans;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			ans.value[y * 4 + x] = value[y * 4 + 0] * rhs.value[x + 0]
								 + value[y * 4 + 1] * rhs.value[x + 4]
								 + value[y * 4 + 2] * rhs.value[x + 8]
								 + value[y * 4 + 3] * rhs.value[x + 12];
		}
	}
	return ans;
}

Matrix4 Matrix4::operator*=(Matrix4 rhs)
{
	*this = rhs * *this;
	return *this;
}

Matrix4 Matrix4::scale(float x, float y, float z) {
	Matrix4 m = Matrix4();
	m.value[0] = x;
	m.value[5] = y;
	m.value[10] = z;
	m.value[15] = 1;
	return m;
}

Matrix4 Matrix4::translate(float x, float y, float z) {
	Matrix4 m = Matrix4::identity();
	m.value[3] = x;
	m.value[7] = y;
	m.value[11] = z;
	return m;
}

Matrix4 Matrix4::rotation(float x, float y, float z) {
	Matrix4 m = Matrix4::identity();
	m.value[0] = cos(y); 
	m.value[2] = sin(y);
	m.value[8] = -sin(y);
	m.value[10] = cos(y);
	return m;
}

Matrix4 Matrix4::identity() {
	Matrix4 m = Matrix4();
	m.value[0] = 1;
	m.value[5] = 1;
	m.value[10] = 1;
	m.value[15] = 1;
	return m;
}
