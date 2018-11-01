#pragma once

#include "Matrix4.h"
#include <iostream>

class Camera
{

private:
	//View matrix in the MVP
	Matrix4 translate = Matrix4::identity();
	float yaw = 0;
	float pitch = 0;
	

public:
	


	Camera() {};
	~Camera() {};

	void move(float x, float y, float z) { translate *= Matrix4::translate(-x, -y, -z); }
	void rollYaw(float x) { yaw += x; }
	void rollPitch(float x) { pitch += x; }

	Matrix4 getTransform() { return  Matrix4::rotationY(yaw) * Matrix4::rotationX(pitch) * translate; }

	friend class Rasteriser;	//Rasteriser can acess m directly.
};

