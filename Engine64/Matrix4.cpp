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
			ans.value[y * 4 + x] = value[x + 0]  * rhs.value[y * 4 + 0]
								 + value[x + 4]  * rhs.value[y * 4 + 1]
								 + value[x + 8]  * rhs.value[y * 4 + 2]
								 + value[x + 12] * rhs.value[y * 4 + 3];
		}
	}
	return ans;
}

Matrix4 Matrix4::scale(float x, float y, float z) {
	Matrix4 m = Matrix4();
	m.value[0] = x;
	m.value[5] = y;
	m.value[10] = z;
	m.value[15] = 1;
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
