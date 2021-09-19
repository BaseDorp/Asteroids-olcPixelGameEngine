#pragma once
#include <vector>
#include "SpaceObject.h"
#include "olcPixelGameEngine.h"

class Asteroid : public SpaceObject
{
public:
	std::vector<std::pair<float, float>> vertices;
	void CreateAsteroid(int numOfVertices);

	float x, y; // position
	float dx, dy; // direction
	float ax, ay; // TODO dont want this if i dont have to
	float angle; // rotation
	float size; // radius
	float minSize;
	float spinRate;

	Asteroid(); // dont use default constructor
	Asteroid(float x, float y, float dx, float dy, int size, float angle);
	Asteroid(olc::PixelGameEngine* instance);
	void SplitAsteroid(std::vector<Asteroid*> &asteroids);
	void ExplosionEffect(olc::PixelGameEngine* instance);
};