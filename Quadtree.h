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
			 // find the point on the AABB that is closest to the circle
			 //if the distance from the circle to this point is less than its radius, we have a collision.

			 //get distance from center of circle to center of rectangle
			/*float distanceX = cx - (this->width/2 + this->x);
			float distanceY = cy - (this->height/2 + this->y);*/
			

			// get the point on the rectangle that is closest to the center of the circle
			float closestX = clamp(cx - (this->x + (width/2)), this->x, this->x + width);
			float closestY = clamp(cy - (this->y + (height/2)), this->y, this->y + height);

			float distance = (closestX * closestX) + (closestY * closestY);

			return (distance < r * r);

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
	/*Quadtree* topRight;
	Quadtree* topLeft;
	Quadtree* bottomRight;
	Quadtree* bottomLeft;*/

	Quadtree(Rectangle* rectangle);
	void Split(); // Splits the Quadtree into 4 subnodes
	void Insert(SpaceObject* spaceObject);
	void Delete(SpaceObject* spaceObject);
	void Clear(); // Clears all the objects from this Quadtree down recursively
	void Draw(olc::PixelGameEngine* instance);

	void Query(Rectangle* area); // find objects in a certain area
};

