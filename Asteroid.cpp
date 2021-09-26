#include "Asteroid.h"

Asteroid::Asteroid(float x, float y, float vx, float vy, int size, float angle)
{
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	this->size = size;
	this->angle = angle;
	this->spinRate = rand() % 5;
	CreateAsteroid(20);
}

Asteroid::Asteroid(olc::PixelGameEngine* instance)
{
	// put random asteroid on screen
	// TODO make them not spawn on player?
	// right now it just keeps doing a new game becuse the player dies when they touch an asteroid. But will have to replace when I implement a game over screen
	x = rand() % instance->ScreenWidth();
	y = rand() % instance->ScreenHeight();
	vx = 1 + rand() % 15;
	vy = 1 + rand() % 15;
	angle = 0.0f;
	size = 16;
	spinRate = rand() % 5;
	CreateAsteroid(20);
}

void Asteroid::CreateAsteroid(int numOfVertices)
{
	 //creating an asteroid of random size
	for (int i = 0; i < numOfVertices; i++)
	{
		float radius = size;
		//float radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;  // TODO dont understand the numbers here
		float a = ((float)i / (float)numOfVertices) * 6.28318f; // percent of current index to how many verts * 2pi
		vertices.push_back(std::make_pair(radius * sinf(a), radius * cos(a)));
	}
}

void Asteroid::UpdateAsteroid(float fElaspedTime)
{
	//acceleration = all forces added together / mass
	// have to make sure to reset acceleration on update so the acceleration doesnt stack

	// using size as the mass
	this->vx += (this->ax /*/ this->size*/) * fElaspedTime;
	this->vy += (this->ay /*/ this->size*/) * fElaspedTime;

	this->x += this->vx * fElaspedTime;
	this->y += this->vy * fElaspedTime;
	this->angle += this->spinRate * fElaspedTime;

	this->ax = 0; // TODO dont know if i have to reset the acceleration after computations
	this->ay = 0;
}

void Asteroid::SplitAsteroid(std::vector<Asteroid*> &asteroids)
{
	// Create 2 new asteroids half the size of the original going in different directions
	// new asteroids should be going in opposite directions

	//float randAngle = rand() % 360; // random number between 0-359? // TODO make it to where the new asteroids go in random direction

	asteroids.push_back(new Asteroid(this->x, this->y, this->vx, -this->vy, this->size/2, this->angle));
	asteroids.push_back(new Asteroid(this->x, this->y, -this->vx, this->vy, this->size / 2, this->angle));
}

void Asteroid::ExplosionEffect(olc::PixelGameEngine* instance)
{
	// TODO
}