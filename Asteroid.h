#pragma once
#include <vector>
#include "SpaceObject.h"
#include "olcPixelGameEngine.h"

class Asteroid : public SpaceObject
{
public:
	std::vector<std::pair<float, float>> vertices;
	void CreateAsteroid(int numOfVertices);

	float x, y;
	float dx, dy;
	float angle;
	float size;
	float minSize;
	float spinRate;

	// dont use
	Asteroid();
	Asteroid(float x, float y, float dx, float dy, int size, float angle);
	Asteroid(olc::PixelGameEngine* instance);
	void SplitAsteroid(std::vector<Asteroid*> &asteroids);
};