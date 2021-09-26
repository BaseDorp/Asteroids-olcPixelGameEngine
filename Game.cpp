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
	bool bDebugMode = true;

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
		// TODO drawstring(score)
		DrawString(0, 10, "Accuracy: " + std::to_string(player.GetAccuracy()));

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


		// Updating Asteroid
		for (auto a1 : Asteroids)
		{
			for (auto a2 : Asteroids)
			{
				if (a1 != a2) // makes sure collision isnt check with itself
				{
					// if the two asteroids do collide
					if (AreCirclesOverlapping(a1->x, a1->y, a1->size, a2->x, a2->y, a2->size))
					{
						// TODO

						// get point where vectors hit, reflect off of that point?

						//DrawLine(a->x, a->y, otherA->x, otherA->y);
						

						// Distance between asteroids
						float fDistance = sqrtf((a1->x - a2->x) * (a1->x - a2->x) + (a1->y - a2->y) * (a1->y - a2->y));

						// gets how much the circles are overlapping. divided by 2 because we only assert half of the displacement onto each circle
						float fOverlap = 0.5f * (fDistance - a1->size - a2->size);
					}
				}
			}

			// Update positions
			a1->UpdateAsteroid(fElapsedTime);
			WrapCoordinates(a1->x, a1->y, a1->x, a1->y);
			DrawWireFrameModel(a1->vertices, a1->x, a1->y, a1->angle, a1->size/a1->size); // TODO something wrong with the that i dont know how to fix yet

			// TODO collision between asteroids
			// check overlaps of cirlce
			// get overlap amount
			// do math to set direction vectors
			// set directions to reflect off point of contact?
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
			b->x += b->vx * fElapsedTime;
			b->y += b->vy * fElapsedTime;
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

					Asteroids.erase(Asteroids.begin()+i); // TODO pretty sure there is a better way to do this
					i--; // think i have to do this to make sure I dont skip the next asteroid when I remove this
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

		// Check if player destroyed all asteroids aka LEVEL COMPLETE
		if (Asteroids.empty())
		{
			Bullets.clear();

			// TODO 
		}

		if (GetKey(olc::Key::D).bPressed)
		{
			bDebugMode = !bDebugMode;
		}
		if (bDebugMode)
		{
			// TODO
			for (auto a : Asteroids)
			{
				// shows radius of circles
				DrawCircle(a->x, a->y, a->size);
			
				// shows which circles are colliding
				for (auto otherA : Asteroids)
				{
					if (a != otherA)
					{
						//TODO something is wrong with the radius
						if (AreCirclesOverlapping(a->x, a->y, a->size/2, otherA->x, otherA->y, otherA->size/2))
						{
							DrawLine(a->x, a->y, otherA->x, otherA->y, olc::RED);
						}
					}
				}
				
			}
		}

		return true;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="x1"></param>
	/// <param name="y1"></param>
	/// <param name="r1"></param>
	/// <param name="x2"></param>
	/// <param name="y2"></param>
	/// <param name="r2"></param>
	/// <returns></returns>
	bool AreCirclesOverlapping(float x1, float y1, float r1, float x2, float y2, float r2)
	{
		return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 * r2);
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
	void DrawWireFrameModel(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float a = 0.0f, float s = 1.0f, olc::Pixel color = olc::WHITE)
	{
		// Create a vector of the newly translated vertices
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		// Rotation
		for (int i = 0; i < verts; i++)
		{
			// Multiplies each vertex
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(a) - vecModelCoordinates[i].second * sinf(a);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(a) + vecModelCoordinates[i].second * cosf(a);
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

		// Draw lines from each vertex
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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="p"></param>
	/// <returns></returns>
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
		for (int i = 0; i < 10; i++)
		{
			Asteroids.push_back(new Asteroid(this));
		}

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