#pragma once

class SpaceObject
{
public:
	float x;
	float y;
	float dx;
	float dy;

	int size;
	float angle;

	SpaceObject();
	SpaceObject(float x, float y, float dx, float dy, int size, float angle);
};