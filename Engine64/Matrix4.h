#pragma once
#include <math.h>
#include <iostream>


const float PI = 3.14159265359;

//Have to do multiplications backwards
class Matrix4
{
private:

public:
	float value[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	Matrix4();
	~Matrix4();
	Matrix4 operator* (Matrix4 rhs);
	Matrix4 operator*= (Matrix4 rhs);


	static Matrix4 identity();
				   
	static Matrix4 rotationY(float x);
	static Matrix4 rotationX(float x);
	static Matrix4 rotationZ(float x);
				   
	static Matrix4 scale(float x, float y, float z);
	static Matrix4 translate(float x, float y, float z);
				   
	static Matrix4 Perspective(float znear, float zfar, float fov, float aspect);
	
	float &operator[] (int x) { return value[x]; }

	friend std::ostream &operator<<(std::ostream &o, Matrix4 m);

};

inline Matrix4 Matrix4::operator*(Matrix4 rhs)
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

inline Matrix4 Matrix4::operator*=(Matrix4 rhs)
{
	*this = rhs * *this;
	return *this;
}

inline Matrix4 Matrix4::scale(float x, float y, float z) {
	Matrix4 m = Matrix4();
	m.value[0] = x;
	m.value[5] = y;
	m.value[10] = z;
	m.value[15] = 1;
	return m;
}

inline Matrix4 Matrix4::translate(float x, float y, float z) {
	Matrix4 m = Matrix4::identity();
	m.value[3] = x;
	m.value[7] = y;
	m.value[11] = z;
	return m;
}



inline Matrix4 Matrix4::rotationY(float x) {
	Matrix4 m = Matrix4::identity();
	m.value[0] = cos(x);
	m.value[2] = sin(x);
	m.value[8] = -sin(x);
	m.value[10] = cos(x);
	return m;
}

inline Matrix4 Matrix4::rotationX(float x) {
	Matrix4 m = Matrix4::identity();
	m.value[5] = cos(x);
	m.value[6] = -sin(x);
	m.value[9] = sin(x);
	m.value[10] = cos(x);
	return m;
}

inline Matrix4 Matrix4::rotationZ(float x) {
	Matrix4 m = Matrix4::identity();
	m.value[0] = cos(x);
	m.value[1] = -sin(x);
	m.value[4] = sin(x);
	m.value[5] = cos(x);
	return m;
}

inline Matrix4 Matrix4::identity() {
	Matrix4 m = Matrix4();
	m.value[0] = 1;
	m.value[5] = 1;
	m.value[10] = 1;
	m.value[15] = 1;
	return m;
}


inline Matrix4 Matrix4::Perspective(float znear, float zfar, float fov, float aspect)
{
	Matrix4 m;
	float f = atan(fov / 2);
	m.value[0] = f / aspect;
	m.value[5] = f;
	m.value[10] = (znear + zfar) / (znear - zfar);
	m.value[11] = (2 * znear * zfar) / (znear - zfar);
	m.value[14] = -1;

	return m;
}