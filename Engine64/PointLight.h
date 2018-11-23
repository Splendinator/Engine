#pragma once

#include "Vector.h"

class PointLight
{
public:

	float rad;
	Vector4 col;
	Vector3 pos;

	PointLight() {};

	PointLight(float radius, Vector4 colour, Vector3 position) { rad = radius; col = colour; pos = position; };
	~PointLight();
};

