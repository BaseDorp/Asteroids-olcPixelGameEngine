#pragma once
#include <vector>
#include "SpaceObject.h"
#include "olcPixelGameEngine.h"

class Asteroid : public SpaceObject
{
public:
	std::vector<std::pair<float, float>> vertices;
	void CreateAsteroid(int numOfVertices);
	float spinRate;

	Asteroid(); // dont use default constructor
	Asteroid(float x, float y, float vx, float vy, int size, float angle);
	Asteroid(olc::PixelGameEngine* instance);
	void SplitAsteroid(std::vector<Asteroid*> &asteroids);
	void ExplosionEffect(olc::PixelGameEngine* instance);
	void UpdateAsteroid(float fElaspedTime);
};