#pragma once
#include "SpaceObject.h"
class Powerup : public SpaceObject
{
public:
	enum Powerups
	{
		None,
		Shield,
		Shotgun
	};
	Powerups Type;

	float shieldSize;

	Powerup();
};

