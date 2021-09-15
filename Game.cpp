#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Player.h"
#include "Asteroid.h"
#include "SpaceObject.h"

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Asteroids";
	}

private:
	//struct SpaceObject
	//{
	//	float x, y; // Position
	//	float dx, dy; // Direction
	//	int size;
	//	float angle;
	//};

	std::vector<Asteroid*> Asteroids;
	std::vector<SpaceObject*> Bullets;
	Player player;
	int Score = 0;

	/*std::vector<std::pair<float, float>> vecPlayerShip;
	std::vector<std::pair<float, float>> vecAsteroids;*/

public:
	bool OnUserCreate() override // Start
	{
		player.vertices = // TODO idk why the vertices arent initializing
		{
			{0.0f, -5.0f},
			{-2.5f, 2.5f},
			{2.5f, 2.5f}
		};

		ResetGame();	

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override // Update
	{
		// Resets the screen to black
		Clear(olc::BLACK);

		if (player.bDead)
		{
			ResetGame();
		}

		player.UpdateInput(this, fElapsedTime);

		// Shooting // put this in the game loop here because it is easier to connect to the bullets vector. should be in player class tbh
		if (this->GetKey(olc::Key::SPACE).bPressed)
		{
			Bullets.push_back(new SpaceObject(player.x, player.y, 50.0f * sinf(player.angle), -50.0f * cosf(player.angle), 0, 0));
			//Bullets.push_back(({ player.x, player.y, 50.0f * sinf(player.angle), -50.0f * cosf(player.angle), 0, 0 }); // TODO make 50 its own variables
			player.shotsFired++;
		}

		// Wrap the player in the screen
		WrapCoordinates(player.x, player.y, player.x, player.y);

		// Drawing the player vertices // TODO should probably use DrawTriangle() instead
		DrawWireFrameModel(player.vertices, player.x, player.y, player.angle);
		
		// Updating Asteroid
		for (auto a : Asteroids)
		{
			// Update positions
			a->x += a->dx * fElapsedTime;
			a->y += a->dy * fElapsedTime;
			a->angle += a->spinRate * fElapsedTime;
			WrapCoordinates(a->x, a->y, a->x, a->y);
			DrawWireFrameModel(a->vertices, a->x, a->y, a->angle, a->size);
		}

		// Check if player collides with asteroid
		for (auto& a : Asteroids)
		{
			if (IsPointInsideCirle(a->x, a->y, a->size, player.x, player.y))
			{
				player.Died();
			}
		}

		// Asteroids created after colliding with a bullet
		/*std::vector<Asteroid*> NewAsteroids;*/

		// Drawing Bullets and updating position
		for (auto& b : Bullets)
		{
			// Update positions
			b->x += b->dx * fElapsedTime;
			b->y += b->dy * fElapsedTime;
			WrapCoordinates(b->x, b->y, b->x, b->y);
			Draw(b->x, b->y);

			// Check bullets collision with asteroids
			for (int i = 0; i < Asteroids.size(); i++)
			{
				if (IsPointInsideCirle(Asteroids[i]->x, Asteroids[i]->y, Asteroids[i]->size, b->x, b->y))
				{
					Score++;
					b->x = -100; // remove bullet

					// splits the asteroid if it has not been reduced to a certain size
					if (Asteroids[i]->size > Asteroids[i]->size / 2) //  TODO make 4 its own variable // TODO doesnt work
					{
						Asteroids[i]->SplitAsteroid(Asteroids);
					}

					Asteroids.erase(Asteroids.begin()+i); // TODO pretty sure this is ineffiecient
					i--; // think i have to do this to make sure I dont skip the next asteroid when I remove
				}
			}
		}

		/*for (auto a : NewAsteroids)
		{
			Asteroids.push_back(a);
		}*/

		// Remove any bullets that go off screen
		if (Bullets.size() > 0)
		{
			auto i = remove_if(Bullets.begin(), Bullets.end(), [&](SpaceObject* o) { return (o->x < 1 || o->y < 1 || o->x >= ScreenWidth() - 1 || o->y >= ScreenHeight() - 1); });
			if (i != Bullets.end())
			{
				Bullets.erase(i);
			}
		}
		
		// Remove dead asteroids
		/*if (Asteroids.size() > 0)
		{
			auto i = remove_if(Asteroids.begin(), Asteroids.end(), [&](Asteroid* o) { return (o->x < 0); });
			if (i != Asteroids.end())
			{
				Asteroids.erase(i);
			}
		}*/

		// Check if player destroyed all asteroids
		if (Asteroids.empty())
		{
			//Bullets.clear();
			// LEVEL COMPLETE

			// TODO make adding more asteroids its own function //  TODO also dont know what these numbers do
			/*Asteroids.push_back({ 
				30.0f * sinf(player.angle - 3.14159f / 2.0f),
				30.0f * cosf(player.angle - 3.14159 / 2.0f),
				10.0f * sinf(player.angle), 
				10.0f * cosf(player.angle),
				(int)16, 0.0f });
			Asteroids.push_back({ 
				30.0f * sinf(player.angle - 3.14159f / 2.0f),
				30.0f * cosf(player.angle - 3.14159 / 2.0f),
				10.0f * sinf(-player.angle), 
				10.0f * cosf(-player.angle),
				(int)16, 0.0f });*/
		}

		return true;
	}

	/// <summary>
	/// Checks if a given point is inside a circle
	/// </summary>
	/// <param name="cx"> - Circle X Position</param>
	/// <param name="cy"> - Circle Y Position</param>
	/// <param name="radius"> - Circle Radius</param>
	/// <param name="x"> - Point X Position</param>
	/// <param name="y"> - Point Y Position</param>
	/// <returns></returns>
	bool IsPointInsideCirle(float cx, float cy, float radius, float x, float y)
	{
		return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
	}

	/// <summary>
	/// Draws an outline wireframe model from a vector of vertices
	/// </summary>
	/// <param name="vecModelCoordinates"> - vector of vertices </param>
	/// <param name="x"> - x position of the object </param>
	/// <param name="y"> - y position of the object </param>
	/// <param name="r"> - angle rotation of the object </param>
	/// <param name="s"> - scale of the object </param>
	/// <param name="color"> - color of the wireframe</param>
	void DrawWireFrameModel(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, olc::Pixel color = olc::WHITE)
	{
		// Create a vector of the newly translated vertices
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		// Rotation
		for (int i = 0; i < verts; i++)
		{
			// Multiplies each vertex
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

		// Draw lines from each vertex to form the triangle // TODO should probably use DrawTriangle()
		for (int i = 0; i < verts + 1; i++)
		{
			int j = (i + 1);
			DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second);
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="ix"></param>
	/// <param name="iy"></param>
	/// <param name="ox"></param>
	/// <param name="oy"></param>
	void WrapCoordinates(float ix, float iy, float& ox, float& oy)
	{
		// set the output to the input
		ox = ix;
		oy = iy;

		// wraps the pixels to the other side of the screen // TODO dont really know how this works
		if (ix < 0.0f)
		{
			ox = ix + (float)ScreenWidth();
		}
		if (ix >= (float)ScreenWidth())
		{
			ox = ix - (float)ScreenWidth();
		}
		if (iy < 0.0f)
		{
			oy = iy + (float)ScreenHeight();
		}
		if (iy >= (float)ScreenHeight())
		{
			oy = iy - (float)ScreenHeight();
		}
	}

	virtual bool Draw(int32_t x, int32_t y, olc::Pixel p = olc::WHITE)
	{
		float fx, fy;
		WrapCoordinates(x, y, fx, fy);
		olc::PixelGameEngine::Draw(fx, fy);
		return true;
	}

	/// <summary>
	/// 
	/// </summary>
	void ResetGame()
	{
		// remove all asteroids and bullets
		Asteroids.clear();
		//Bullets.clear();

		// Creates asteroid
		Asteroids.push_back(new Asteroid());
		//Asteroids.push_back(new Asteroid());

		// Initialize player in the middle of the screen
		player.x = ScreenWidth() / 2.0f;
		player.y = ScreenHeight() / 2.0f;
		player.dx = 0.0f;
		player.dy = 0.0f;
		player.angle = 0.0f;

		Score = 0;

		player.bDead = false;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(640, 360, 2, 2))
		demo.Start(); // If the window is created, start the application

	return 0;
}