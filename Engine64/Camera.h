#pragma once

#include "Matrix4.h"
#include <iostream>
#include "Vector.h"
#include <math.h>

class Camera
{

private:
	//View matrix in the MVP
	Matrix4 translate = Matrix4::identity();
	float yaw = 0;
	float pitch = 0;

	
public:
	
	

	Camera() { };
	~Camera() {};

	void move(Vector3 v) { translate *= Matrix4::translate(v[0], v[1], v[2]); }
	void rollYaw(float x) { yaw += x; }
	void rollPitch(float x) { pitch += x; }

	Matrix4 getTransform() { return   Matrix4::rotationX(pitch) * Matrix4::rotationY(yaw) * translate; }

	Vector3 foward();
	Vector3 right();
	
	
};




inline Vector3 Camera::foward() {
	Vector3 v;
	float f = cos(pitch);

	v[0] = -sin(yaw);
	v[2] = cos(yaw);
	
	v[1] = sin(pitch);
	v[0] *= f;
	v[2] *= f;

	return v.normalise();
}

inline Vector3 Camera::right() {
	Vector3 v;
	
	v[0] = cos(yaw);
	v[2] = sin(yaw);

	return v.normalise();
}