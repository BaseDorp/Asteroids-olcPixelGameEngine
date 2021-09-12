#pragma once
#include <vector>

class Asteroid
{
public:
	std::vector<std::pair<float, float>> vertices;
	void CreateAsteroid();

	float x, y;
	float dx, dy;
	float angle;
	float size;

	Asteroid();
};

