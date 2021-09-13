#pragma once
#include <vector>
#include "SpaceObject.h"
#include "olcPixelGameEngine.h"

class Player : public SpaceObject
{
public:
	std::vector<std::pair<float, float>> vertices;
	int shotsFired;
	bool bDead;

	Player();
	void UpdateInput(olc::PixelGameEngine* Instance, float fElapsedTime);
	void Died();
};

