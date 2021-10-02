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

		// Checks if the point is inside this rectangle
		bool Contains(float x, float y)
		{
			return (x > this->x && y > this->y && x < (this->x + width) && y < (this->y + height));
		}
	};

	int maxObjects; // max amount of objects the quadtree can hold
	int maxLevels; // deepest subnode
	int level; // how deep down this quadtree is. 0 being the root
	bool bIsSplit; // true is this quadtree has split already
	Rectangle* bounds;
	std::vector<SpaceObject*> objects; // TODO make this a template class so it can accept any type of object
	std::vector<Quadtree*> nodes; // leaf subnodes
	/*Quadtree* topRight;
	Quadtree* topLeft;
	Quadtree* bottomRight;
	Quadtree* bottomLeft;*/

	Quadtree(Rectangle* rectangle, int level);
	void Split(); // Splits the Quadtree into 4 subnodes
	void Insert(SpaceObject* spaceObject);
	void Clear(); // Clears all the objects from this Quadtree down recursively
	void Draw(olc::PixelGameEngine* instance);

	void Query(Rectangle* area); // find objects in a certain area
};

