#pragma once
#include <vector>
#include "SpaceObject.h"

class Asteroid : public SpaceObject
{
public:
	std::vector<std::pair<float, float>> vertices;
	void CreateAsteroid(int numOfVertices);

	float x, y;
	float dx, dy;
	float angle;
	float size;
	float spinRate;

	Asteroid(float x, float y, float dx, float dy, int size, float angle);
	Asteroid();
	void SplitAsteroid(std::vector<Asteroid*> &asteroids);
};