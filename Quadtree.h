#pragma once
#include <vector>
#include "SpaceObject.h"

class Quadtree
{
public:
	struct Rectangle
	{
		float x;
		float y;
		float width;
		float height;

		Rectangle(float x, float y, float width, float height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		bool Contains(float x, float y)
		{
			return (x > this->x && y > this->y && x < (this->x + width) && y < (this->y + height));
		}
	};

	int maxObjects; // max amount of objects the quadtree can hold
	int maxLevels; // deepest subnode
	int level; // how deep down this quadtree is. 0 being the root
	bool bIsSplit;
	Rectangle* bounds; // TODO dont think this needs to be heap allocated
	std::vector<SpaceObject*> objects;
	std::vector<Quadtree*> nodes;
	// TODO switch this to nodes
	Quadtree* topRight;
	Quadtree* topLeft;
	Quadtree* bottomRight;
	Quadtree* bottomLeft;

	Quadtree();
	Quadtree(Rectangle rectangle);
	void Split(); // Splits the Quadtree into 4 subnodes
	int GetIndex(); // checks to see which node the object fits in
	void Insert(SpaceObject* spaceObject);
	void Clear(); // Clears all the objects from this Quadtree down recursively
	std::vector<SpaceObject*> Retrieve(std::vector<SpaceObject*> returnObjects, Rectangle rectangle);
};

