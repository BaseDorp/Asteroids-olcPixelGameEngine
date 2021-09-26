#pragma once
#include <vector>
#include "SpaceObject.h"
#include "Powerup.h"
#include "olcPixelGameEngine.h"

class Player : public SpaceObject
{
public:
	std::vector<std::pair<float, float>> vertices;
	int shotsFired;
	int shotsHit;
	bool bDead;
	Powerup powerup;

	Player();
	Player(float x, float y, float vx, float vy, int size, float angle);
	void UpdateInput(olc::PixelGameEngine* Instance, float fElapsedTime);
	void ResetPlayer(olc::PixelGameEngine* instance);
	float GetAccuracy();
	void Died();
};

