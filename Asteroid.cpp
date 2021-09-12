#include "Asteroid.h"

Asteroid::Asteroid()
{
	x, y = 50.0f;
	dx = -5.0f;
	dy = 8.0f;
	angle = 0.0f;
	size = 16;
	CreateAsteroid();
}

void Asteroid::CreateAsteroid()
{
	 //creating an asteroid of random size
	int verts = 20;
	for (int i = 0; i < verts; i++)
	{
		float radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;  // TODO dont understand the numbers here
		float a = ((float)i / (float)verts) * 6.28318f; // percent of current index to how many verts * 2pi
		vertices.push_back(std::make_pair(radius * sinf(a), radius * cos(a)));
	}
}