#include "Player.h"

Player::Player()
{
	std::vector<std::pair<float, float>> verticies =
	{
		{0.0f, -5.0f},
		{-2.5f, 2.5f},
		{2.5f, 2.5f}
	};
	x, y, vx, vy, angle = 0.0f;
	shotsFired = 0;
	shotsHit = 0;
	bDead = false;
	powerup.Type = powerup.None;
}

void Player::UpdateInput(olc::PixelGameEngine* Instance, float fElapsedTime)
{
	// each of these should probably be there own function but not really required since i have no plans on moving this to some other engine

	// Steer the player
	if (Instance->GetKey(olc::Key::LEFT).bHeld)
	{
		this->angle -= 5.0f * fElapsedTime;
	}
	if (Instance->GetKey(olc::Key::RIGHT).bHeld)
	{
		this->angle += 5.0f * fElapsedTime;
	}

	// Thrust
	if (Instance->GetKey(olc::Key::UP).bHeld)
	{
		// Changing the Velocity by 'x' Acceleration
		vx += sin(angle) * 20.0f * fElapsedTime; // TODO I dont get this
		vy -= cos(angle) * 20.0f * fElapsedTime; // TODO change 20.0f to its own variable
	}

	// Updating the position from the velocity
	x += vx * fElapsedTime;
	y += vy * fElapsedTime;
}

void Player::ResetPlayer(olc::PixelGameEngine* instance)
{
	this->x = instance->ScreenWidth() / 2.0f;
	this->y = instance->ScreenHeight() / 2.0f;
	this->vx = 0.0f;
	this->vy = 0.0f;
	this->angle = 0.0f;
	this->bDead = false;
	this->shotsFired = 0;
	this->shotsHit = 0;
}

float Player::GetAccuracy()
{
	if (shotsFired != 0)
	{
		return (float)shotsHit / (float)shotsFired;
	}
	return 0;
}

void Player::Died()
{
	this->bDead = true;
}