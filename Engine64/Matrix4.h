#pragma once
#include <math.h>


const float PI = 3.14159265359;

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

	static Matrix4 rotation(float x, float y, float z);
	static Matrix4 scale(float x, float y, float z);
	static Matrix4 translate(float x, float y, float z);

	
};

