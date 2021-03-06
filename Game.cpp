#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "player.h"
#include "Asteroid.h"
#include "SpaceObject.h"
#include "Quadtree.h"

// around 65mb before with 10 asteroids

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
	std::vector<std::pair<SpaceObject*, SpaceObject*>> CollidingObjects;
	Player* player = nullptr;
	Quadtree* quadtree;
	int Score = 0;
	bool bDebugMode = false;
	bool btestMode = false;

public:
	bool OnUserCreate() override // Start
	{
		player = new Player();
		quadtree = new Quadtree(Quadtree::Rectangle(0, 0, ScreenWidth(), ScreenHeight()));

		ResetGame();	

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override // Update
	{
#pragma region PRE_GAME_CHECKS

		if (this->GetKey(olc::Key::H).bPressed)
		{
			btestMode = !btestMode;
		}

#pragma endregion
#pragma region DRAWING_TO_SCREEN

		// Resets the screen to black
		Clear(olc::BLACK);

		// Drawing stats
		DrawString(0, 5, "Score: " + std::to_string(Score));
		DrawString(0, 15, "Accuracy: " + std::to_string(player->GetAccuracy()));
		DrawString(0, 25, "Press D to toggle debug drawings");

#pragma endregion


		

		if (player->bDead)
		{
			ResetGame();
		}

		player->UpdateInput(this, fElapsedTime);
		
		

		// Shooting // put this in the game loop here because it is easier to connect to the bullets vector. should be in player class tbh
		if (this->GetKey(olc::Key::SPACE).bPressed)
		{
			Bullets.push_back(new SpaceObject(player->x, player->y, 50.0f * sinf(player->angle), -50.0f * cosf(player->angle), 0, 0));
			player->shotsFired++;
		}

		// Wrap the player in the screen
		WrapCoordinates(player->x, player->y, player->x, player->y);
		// Drawing the player vertices
		DrawWireFrameModel(player->vertices, player->x, player->y, player->angle);


		CollidingObjects.clear();
		// Remaking the quadtree each loop to update the positions // TODO
		quadtree->Clear();

		

		for (auto a : Asteroids)
		{
			quadtree->Insert(a, CollidingObjects);
			// TODO when asteroids are getting wrapped to the other side, it will not check the collision because the center point is still on the other side of the screen

			// Update positions
			a->UpdateAsteroid(fElapsedTime);
			WrapCoordinates(a->x, a->y, a->x, a->y);
			DrawCircle(a->x, a->y, a->size);
			//DrawWireFrameModel(a1->vertices, a1->x, a1->y, a1->angle, a1->size/a1->size);

			// TODO collision physics between asteroids
		}
	

		// Check if player collides with asteroid
		//for (auto& a : Asteroids)
		//{
		//	if (IsPointInsideCirle(a->x, a->y, a->size, player->x, player->y))
		//	{
		//		//player->Died();
		//	}
		//}

		// Drawing Bullets and updating position
		for (auto b : Bullets)
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
					player->shotsHit++; // had to make it shotsHit because of how the game handles removing bullets
					b->x = -100; // remove bullet

					// splits the asteroid if it has not been reduced to a certain size
					if (Asteroids[i]->size > 4) // TODO 4 should be the maxSize / numberOfTimesSplit
					{
						Asteroids[i]->ExplosionEffect(this);
						Asteroids[i]->SplitAsteroid(Asteroids, *quadtree); // TODO remove the dependancy from the quadtree class
					}

					quadtree->Delete(Asteroids[i]);
					Asteroids.erase(Asteroids.begin()+i);
					i--; // make sure I dont skip the next asteroid when I remove this
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
			quadtree->Clear();

			// TODO level complete
		}

#pragma region DEBUG_CODE

		if (GetKey(olc::Key::D).bPressed)
		{
			bDebugMode = !bDebugMode;
		}
		if (bDebugMode)
		{
			// show when objects are colliding
			DrawString(0, 40, "Number of Objects Colliding: " + std::to_string(CollidingObjects.size()));
			for (auto o : CollidingObjects)
			{
				DrawLine(o.first->x, o.first->y, o.second->x, o.second->y);
			}

			// shows quadtrees and how many objects are in each
			quadtree->Draw(this);
		}

#pragma endregion

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
		return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2);
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


	// These Functions are from the olcPixelGameEngine youtube channel as part of a tutorial
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
		// TODO i dont really know how this function works

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
	/// overriden draw function to wrap out of bounds pixels
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	virtual bool Draw(int32_t x, int32_t y, olc::Pixel p = olc::WHITE)
	{
		float fx, fy;
		WrapCoordinates(x, y, fx, fy);
		olc::PixelGameEngine::Draw(fx, fy, p);
		return true;
	}

	/// <summary>
	/// 
	/// </summary>
	void ResetGame()
	{ 
		// TODO find cleaner way to free memory in the vectors
		// remove all asteroids and bullets
		for (auto a : Asteroids)
		{
			delete a;
		}
		Asteroids.clear();
		for (auto b : Bullets)
		{
			delete b;
		}
		Bullets.clear();
		quadtree->Clear();

		// Creates asteroid
		srand(time(NULL));
		for (int i = 0; i < 10; i++)
		{
			Asteroid* a = new Asteroid(this);
			Asteroids.push_back(a);
			quadtree->Insert(a, CollidingObjects);
		}

		Score = 0;
		player->ResetPlayer(this);
	}
};


int main()
{
	Example demo;
	if (demo.Construct(400, 400, 2, 2))
		demo.Start(); // If the window is created, start the application

	return 0;
}