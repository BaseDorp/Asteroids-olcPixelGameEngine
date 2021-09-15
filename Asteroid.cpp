#include "Asteroid.h"

Asteroid::Asteroid(float x, float y, float dx, float dy, int size, float angle)
{
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	this->size = size;
	this->angle = angle;
	CreateAsteroid(20);
}

Asteroid::Asteroid()
{
	x, y = 50.0f;
	dx = -5.0f;
	dy = 8.0f;
	angle = 0.0f;
	size = 16;
	CreateAsteroid(20);
}

void Asteroid::CreateAsteroid(int numOfVertices)
{
	 //creating an asteroid of random size
	for (int i = 0; i < numOfVertices; i++)
	{
		float radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;  // TODO dont understand the numbers here
		float a = ((float)i / (float)numOfVertices) * 6.28318f; // percent of current index to how many verts * 2pi
		vertices.push_back(std::make_pair(radius * sinf(a), radius * cos(a)));
	}
}

void Asteroid::SplitAsteroid(std::vector<Asteroid*> &asteroids)
{
	// Create 2 new asteroids half the size of the original going in different directions
	// new asteroids should be going in opposite directions

	//float randAngle = rand() % 360; // random number between 0-359? // TODO make it to where the new asteroids go in random direction

	asteroids.push_back(new Asteroid(this->x, this->y, this->dx, -this->dy, this->size/2, this->angle));
	asteroids.push_back(new Asteroid(this->x, this->y, -this->dx, this->dy, this->size / 2, this->angle));
}