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

	const float MAX_PITCH = (PI / 2);

	
public:
	
	

	Camera() { };
	~Camera() {};

	void move(Vector3 v) { translate *= Matrix4::translate(-v[0], -v[1], -v[2]); }
	void rollYaw(float x) { yaw += x; }
	void rollPitch(float x);

	Matrix4 getTransform() { return   Matrix4::rotationX(pitch) * Matrix4::rotationY(yaw) * translate; }

	Vector3 foward();
	Vector3 right() { return Vector3({ cos(yaw), 0,sin(yaw) }).normalise(); }
	Vector3 up();
	
	Vector3 getPosition() { return Vector3({translate[3],translate[7],translate[11]}); }
	
};




inline void Camera::rollPitch(float x)
{
	pitch += x; 
	if (pitch > MAX_PITCH) pitch = MAX_PITCH;
	else if (pitch < -MAX_PITCH) pitch = -MAX_PITCH;
}

inline Vector3 Camera::foward() {
	float f = cos(pitch);

	return Vector3({sin(yaw) * f, -sin(pitch), -cos(yaw) * f}).normalise();
}

inline Vector3 Camera::up() {
	float f = sin(pitch);

	return Vector3({ sin(yaw) * f, cos(pitch), -cos(yaw) * f }).normalise();
	//return Vector3({ 0,cos(pitch),-sin(pitch) }).normalise();
}