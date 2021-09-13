#pragma once
#include <vector>
#include "SpaceObject.h"

class Asteroid : SpaceObject
{
public:
	std::vector<std::pair<float, float>> vertices;
	void CreateAsteroid();

	float x, y;
	float dx, dy;
	float angle;
	float size;
	float spinRate;

	Asteroid();
	void SplitAsteroid();
};

