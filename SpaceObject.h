#pragma once

class SpaceObject
{
public:
	float x, y; // position
	float vx, vy; // velocity
	float ax, ay; // acceleration
	int size; // radius, mass
	float angle;

	//SpaceObject();
	SpaceObject(float x = 0.0f, float y = 0.0f, float vx = 0.0f, float vy = 0.0f, int size = 0, float angle = 0.0f);
	//SpaceObject(float x, float y, float vx, float vy, int size, float angle);
};