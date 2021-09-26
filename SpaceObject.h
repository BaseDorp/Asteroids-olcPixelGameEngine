#pragma once

class SpaceObject
{
public:
	float x, y; // position
	float vx, vy; // velocity
	float ax, ay; // acceleration
	int size; // radius, mass
	float angle;

	SpaceObject();
	SpaceObject(float x, float y, float vx, float vy, int size, float angle);
};