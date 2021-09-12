#include "Player.h"

Player::Player()
{
	std::vector<std::pair<float, float>> verticies =
	{
		{0.0f, -5.0f},
		{-2.5f, 2.5f},
		{2.5f, 2.5f}
	};

	float x, y, dx, dy, angle = 0.0f;
	int shotsFired = 0;
	bDead = false;
}

void Player::UpdateInput(olc::PixelGameEngine* Instance, float fElapsedTime)
{
	// Steer the player
	if (Instance->GetKey(olc::Key::LEFT).bHeld)
	{
		angle -= 5.0f * fElapsedTime;
	}
	if (Instance->GetKey(olc::Key::RIGHT).bHeld)
	{
		angle += 5.0f * fElapsedTime;
	}

	// Thrust
	if (Instance->GetKey(olc::Key::UP).bHeld)
	{
		// Changing the Velocity by 'x' Acceleration
		dx += sin(angle) * 20.0f * fElapsedTime; // TODO I dont get this
		dy -= cos(angle) * 20.0f * fElapsedTime; // TODO change 20.0f to its own variable
	}

	// Shooting
	if (Instance->GetKey(olc::Key::SPACE).bPressed)
	{
		//Bullets.push_back({ x, y, 50.0f * sinf(angle), -50.0f * cosf(angle), 0, 0 }); // TODO make 50 its own variable
	}
}

	//void DrawPlayer(olc::PixelGameEngine Instance)
	//{
	//	Instance.DrawTriangle(player.x + verticies[1, 1], )
	//}
