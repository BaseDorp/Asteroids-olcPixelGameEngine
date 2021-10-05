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

		Rectangle()
		{
			this->x = 0;
			this->y = 0;
			this->width = 1;
			this->height = 1;
		}

		Rectangle(float x, float y, float width, float height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		// Checks if the circle is inside this rectangle bounds
		bool Contains(float cx, float cy, float r)
		{	
			// get the point on the rectangle that is closest to the center of the circle
			float halfextentsX = this->width / 2.0f;
			float halfextentsY = this->height / 2.0f;

			float centerX = this->x + halfextentsX;
			float centerY = this->y + halfextentsY;

			float differenceX = cx - centerX;
			float differenceY = cy - centerY;

			float clampedX = clamp(differenceX, -halfextentsX, halfextentsX);
			float clampedY = clamp(differenceY, -halfextentsY, halfextentsY);

			float closestX = centerX + clampedX;
			float closestY = centerY + clampedY;

			differenceX = closestX - cx;
			differenceY = closestY - cy;

			return ((differenceX * differenceX) + (differenceY * differenceY) <= r * r);

			// Point collisoin check
			//return (cx > this->x && cy > this->y && cx < (this->x + width) && cy < (this->y + height));
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
	Rectangle* bounds;
	std::vector<SpaceObject*> objects; // TODO make this a template class so it can accept any type of object
	std::vector<Quadtree*> nodes; // leaf subnodes

	Quadtree(Rectangle* rectangle);
	void Split(); // Splits the Quadtree into 4 subnodes
	void Insert(SpaceObject* spaceObject);
	void Delete(SpaceObject* spaceObject);
	void Clear(); // Clears all the objects from this Quadtree down recursively
	void Draw(olc::PixelGameEngine* instance);

	void Query(Rectangle* area); // find objects in a certain area
};

