#include "Quadtree.h"

Quadtree::Quadtree(Rectangle* bounds, int level)
{
	this->maxLevels = 4;
	this->bIsSplit = false;
	this->bounds = bounds;
	this->level = level++;
	this->maxObjects = 4;
}

void Quadtree::Split()
{
	float x = bounds->x;
	float y = bounds->y;
	float subWidth = bounds->width / 2;
	float subHeight = bounds->height / 2;

	// Top Left
	nodes.push_back(new Quadtree(new Rectangle(x, y, subWidth, subHeight), this->level++));
	// Top Right
	nodes.push_back(new Quadtree(new Rectangle(x + subWidth, y, subWidth, subHeight), this->level++));
	// Bottom Left
	nodes.push_back(new Quadtree(new Rectangle(x, y + subHeight, subWidth, subHeight), this->level++));
	// Bottom Right
	nodes.push_back(new Quadtree(new Rectangle(x + subWidth, y + subHeight, subWidth, subHeight), this->level++));

	// takes all the objects in the parent node and splits them into there corresponding subdivided quadtree
	for (auto o : this->objects)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i]->bounds->Contains(o->x, o->y))
			{
				nodes[i]->Insert(o);
			}
		}
	}

	this->bIsSplit = true;
}

void Quadtree::Insert(SpaceObject* spaceObject)
{
	// check if the object is inside this quadtree // for recurssion
	if (!bounds->Contains(spaceObject->x, spaceObject->y))
	{
		return;
	}

	// add the object to the quadtree if the max has not been hit or is at the lowest leaf level
	if (this->objects.size() < maxObjects || this->level == maxLevels) 
	{
		objects.push_back(spaceObject);
	}
	else
	{
		if (!this->bIsSplit) // splits the quadtree if this quadtree has not been split yet
		{
			this->Split();
		}

		for (auto n : nodes)
		{
			n->Insert(spaceObject);
		}
	}
}

void Quadtree::Clear()
{
	if (bIsSplit)
	{
		for (auto n : nodes)
		{
			n->Clear();
			n = nullptr;
		}
	}
	objects.clear();
}

void Quadtree::Draw(olc::PixelGameEngine* instance)
{
	if (!bIsSplit) // makes sure the parent nodes dont draw on top of child nodes
	{
		instance->DrawString(bounds->x + 2, bounds->y + 2, std::to_string(this->objects.size()));
	}
	instance->DrawRect(bounds->x, bounds->y, bounds->width, bounds->height);

	if (bIsSplit)
	{
		for (auto n : nodes)
		{
			n->Draw(instance);
		}
	}
}

//void Quadtree::Query(float range)
//{
//	if (!this-)
//}