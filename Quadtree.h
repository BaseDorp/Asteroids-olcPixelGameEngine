#pragma once
#include <vector>
#include "SpaceObject.h"
#include "olcPixelGameEngine.h"

class Quadtree
{
public:
	struct Rectangle
	{
		float x;
		float y;
		float width;
		float height;

		Rectangle(float x = 0.0f, float y = 0.0f, float width = 1.0f, float height = 1.0f)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		/*
		*	Circle/AABB collision code derived from: https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-Detection
		*/
		// Checks if the circle is inside this rectangle bounds
		bool Contains(float cx, float cy, float r)
		{	
			float halfextentsX = this->width / 2.0f;
			float halfextentsY = this->height / 2.0f;

			float centerX = this->x + halfextentsX;
			float centerY = this->y + halfextentsY;

			float differenceX = cx - centerX;
			float differenceY = cy - centerY;

			// get the point on the rectangle that is closest to the center of the circle
			float clampedX = clamp(differenceX, -halfextentsX, halfextentsX);
			float clampedY = clamp(differenceY, -halfextentsY, halfextentsY);

			float closestX = centerX + clampedX;
			float closestY = centerY + clampedY;

			differenceX = closestX - cx;
			differenceY = closestY - cy;

			return ((differenceX * differenceX) + (differenceY * differenceY) <= r * r);
		}

		float clamp(float value, float min, float max) // not really nessacary for the struct but serperated for readability
		{
			return std::max(min, std::min(max, value));
		}
	};

	int maxObjects; // max amount of objects the quadtree can hold
	// TODO add max about of times the quadtree can split
	//int maxLevels; // amount of times the quadtree can split
	//int level; // how deep down this quadtree is. 0 being the root
	bool bIsSplit; // true is this quadtree has split already
	Rectangle bounds;
	std::vector<SpaceObject*> objects;
	std::vector<std::shared_ptr<Quadtree>> nodes; // leaf subnodes

	Quadtree(Rectangle rectangle);
	// Splits the Quadtree into 4 sub-quadtrees
	void Split(std::vector<std::pair<SpaceObject*, SpaceObject*>>& collidingObjects);
	void Insert(SpaceObject* spaceObject, std::vector<std::pair<SpaceObject*, SpaceObject*>>& collidingObjects);
	void Delete(SpaceObject* spaceObject);
	void Clear(); // Clears all the objects from this Quadtree down recursively
	void Draw(olc::PixelGameEngine* instance);
};

