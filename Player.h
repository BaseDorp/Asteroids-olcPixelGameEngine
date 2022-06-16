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
	//Player(float x = 0.0f, float y = 0.0f, float vx = 0.0f, float vy = 0.0f, int size = 0, float angle = 0.0f);
	void UpdateInput(olc::PixelGameEngine* Instance, float fElapsedTime);
	void ResetPlayer(olc::PixelGameEngine* instance);
	float GetAccuracy();
	void Died();
};

