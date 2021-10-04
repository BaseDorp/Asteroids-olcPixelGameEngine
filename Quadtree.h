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
		bool Contains(float x, float y, float r)
		{
			float testX = x;
			float testY = y;

			// finds which 2 edges to check collision against
			if (x < this->x) // left of the rectangle
			{
				testX = this->x;
			}
			else if (x > this->x + this->width) // right
			{
				testX = this->x + this->width;
			}

			if (y < this->y) // top edge
			{
				testY = this->y;
			}
			else if (y > this->y + this->height) // bottom edge
			{
				testY = this->y + this->height;
			}

			
			return (r >= sqrt(((x - testX) * (x - testX)) + ((y - testY) * (y - testY))));

			//return (x > this->x && y > this->y && x < (this->x + width) && y < (this->y + height));
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

