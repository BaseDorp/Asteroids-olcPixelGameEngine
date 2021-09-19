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
	std::vector<Asteroid*> Asteroids;
	std::vector<SpaceObject*> Bullets;
	Player player;
	int Score = 0;

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
		DrawString(0, 0, "Accuracy: " + std::to_string(player.GetAccuracy()));

		// Shooting // put this in the game loop here because it is easier to connect to the bullets vector. should be in player class tbh
		if (this->GetKey(olc::Key::SPACE).bPressed)
		{
			Bullets.push_back(new SpaceObject(player.x, player.y, 50.0f * sinf(player.angle), -50.0f * cosf(player.angle), 0, 0));
			player.shotsFired++;
		}

		// Wrap the player in the screen
		WrapCoordinates(player.x, player.y, player.x, player.y);

		// Drawing the player vertices // TODO should probably use DrawTriangle() instead
		DrawWireFrameModel(player.vertices, player.x, player.y, player.angle);
		
		// lambda function that checks if two circles are intersecting
		auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2)
		{
			// absolute value // squared the radi to get rid of the sqrt
			return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 * r2);
		};

		// Updating Asteroid
		for (auto a : Asteroids)
		{
			for (auto otherA : Asteroids)
			{
				if (a != otherA) // makes sure collision isnt check with itself
				{
					if (DoCirclesOverlap(a->x, a->y, a->size, otherA->x, otherA->y, otherA->size))
					{
						// TODO idk why we cant remove the sqrt here though
						float distance = sqrt((a->x - otherA->x) * (a->x - otherA->x) + (a->y - otherA->y) * (a->y - otherA->y));

						float overlap = (distance - a->size - otherA->size) * 0.5f;
					}

					
				}
			}

			// Update positions
			a->x += a->dx * fElapsedTime;
			a->y += a->dy * fElapsedTime;
			a->angle += a->spinRate * fElapsedTime;
			WrapCoordinates(a->x, a->y, a->x, a->y);
			DrawWireFrameModel(a->vertices, a->x, a->y, a->angle, a->size);

			// TODO collision between asteroids
			// check overlaps of cirlce
			// get overlap amount
			// do math to set direction vectors
		}

		// Check if player collides with asteroid
		for (auto& a : Asteroids)
		{
			if (IsPointInsideCirle(a->x, a->y, a->size, player.x, player.y))
			{
				player.Died();
			}
		}

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
					player.shotsHit++; // had to make it shotsHit because of how the game handles removing bullets
					b->x = -100; // remove bullet

					// splits the asteroid if it has not been reduced to a certain size
					if (Asteroids[i]->size > 4) // TODO 4 should be the maxSize / numberOfTimesSplit
					{
						Asteroids[i]->ExplosionEffect(this);
						Asteroids[i]->SplitAsteroid(Asteroids);
					}

					Asteroids.erase(Asteroids.begin()+i); // TODO pretty sure this is ineffiecient
					i--; // think i have to do this to make sure I dont skip the next asteroid when I remove
				}
			}
		}

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

		// Check if player destroyed all asteroids aka LEVEL COMPLETE
		if (Asteroids.empty())
		{
			Bullets.clear();

			// TODO make adding more asteroids its own function
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
		Bullets.clear();

		// Creates asteroid
		srand(time(NULL));
		Asteroids.push_back(new Asteroid(this));
		Asteroids.push_back(new Asteroid(this));
		Asteroids.push_back(new Asteroid(this));
		Asteroids.push_back(new Asteroid(this));

		Score = 0;
		player.ResetPlayer(this);
	}
};


int main()
{
	Example demo;
	if (demo.Construct(640, 360, 2, 2))
		demo.Start(); // If the window is created, start the application

	return 0;
}