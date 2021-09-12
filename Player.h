#pragma once
#include <vector>
#include "olcPixelGameEngine.h"

class Player
{
public:
	std::vector<std::pair<float, float>> vertices;

	float x, y;
	float dx, dy;
	float angle;

	int shotsFired;
	bool bDead;

	Player();
	void UpdateInput(olc::PixelGameEngine* Instance, float fElapsedTime);
};

