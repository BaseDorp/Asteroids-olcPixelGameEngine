#include "Quadtree.h"

Quadtree::Quadtree(Rectangle* bounds, int level)
{
	this->maxLevels = 4;
	this->bIsSplit = false;
	this->bounds = bounds;
	this->level = level++;
	this->maxObjects = 3;
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

	// go through objects and see which subnode they belong in
	// add that object to the subnode's objects
	// after all objects have been moved down, clear the parents objects


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
	
	this->objects.clear(); // clears the objects in the parent tree, this isnt really important but saves memeory
	this->bIsSplit = true;
}

void Quadtree::Insert(SpaceObject* spaceObject)
{
	// if the object does not fit inside this quadtree, this isn't the right quadtree
	if (!bounds->Contains(spaceObject->x, spaceObject->y))
	{
		return;
	}

	// add the object to the quadtree if the max has not been hit
	if (!bIsSplit && this->objects.size() < maxObjects) 
	{
		objects.push_back(spaceObject);
		// make this return true so that we dont have to keep checking after its been added
	}
	else
	{
		if (!this->bIsSplit) // splits the quadtree if this quadtree has not been split yet
		{
			this->Split();
		}

		// adds the passed in object to one of the subnodes
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
	else
	{
		for (auto n : nodes)
		{
			n->Draw(instance);
		}
	}

	instance->DrawRect(bounds->x, bounds->y, bounds->width, bounds->height);
}

//void Quadtree::Query(float range)
//{
//	if (!this-)
//}