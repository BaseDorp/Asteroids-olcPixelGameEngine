#pragma once
#include <vector>
#include "SpaceObject.h"
#include "olcPixelGameEngine.h"

class Player : public SpaceObject
{
public:
	std::vector<std::pair<float, float>> vertices;
	int shotsFired;
	int shotsHit;
	bool bDead;

	Player();
	Player(float x, float y, float dx, float dy, int size, float angle);
	void UpdateInput(olc::PixelGameEngine* Instance, float fElapsedTime);
	void ResetPlayer(olc::PixelGameEngine* instance);
	float GetAccuracy();
	void Died();
};

