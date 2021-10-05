#include "Quadtree.h"

Quadtree::Quadtree(Rectangle* bounds)
{
	this->bIsSplit = false;
	this->bounds = bounds;
	this->maxObjects = 5;
}

void Quadtree::Split()
{
	float x = bounds->x;
	float y = bounds->y;
	float subWidth = bounds->width / 2;
	float subHeight = bounds->height / 2;

	// Top Left
	nodes.push_back(new Quadtree(new Rectangle(x, y, subWidth, subHeight)));
	// Top Right
	nodes.push_back(new Quadtree(new Rectangle(x + subWidth, y, subWidth, subHeight)));
	// Bottom Left
	nodes.push_back(new Quadtree(new Rectangle(x, y + subHeight, subWidth, subHeight)));
	// Bottom Right
	nodes.push_back(new Quadtree(new Rectangle(x + subWidth, y + subHeight, subWidth, subHeight)));

	// takes all the objects in the parent node and splits them into there corresponding subdivided quadtree
	for (auto o : this->objects)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i]->bounds->Contains(o->x, o->y, o->size))
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
	if (!bounds->Contains(spaceObject->x, spaceObject->y, spaceObject->size))
	{
		return;
	}

	if (!bIsSplit && this->objects.size() < maxObjects) // add the object to the quadtree if the max has not been hit
	{
		objects.push_back(spaceObject);

		// collision check object with other objects in that quadtree
		//for (auto o : this->objects)
		//{
		//	if (o != spaceObject)
		//	{
		//		float x1 = spaceObject->x;
		//		float y1 = spaceObject->y;
		//		float r1 = spaceObject->size;
		//		float x2 = o->x;
		//		float y2 = o->y;
		//		float r2 = o->size;

		//		if (((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 * r2))
		//		{
		//			// colliding
		//		}
		//	}
		//}
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

void Quadtree::Delete(SpaceObject* spaceObject)
{
	if (!bounds->Contains(spaceObject->x, spaceObject->y, spaceObject->size))
	{
		return;
	}

	if (!bIsSplit)
	{
		// TODO making objects a list instead of a vector would probably be better here
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i] == spaceObject)
			{
				this->objects.erase(objects.begin() + i);
				return;
			}
		}
	}
	else
	{
		for (auto n : nodes)
		{
			n->Delete(spaceObject);
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
		}
		this->bIsSplit = false;
	}
	nodes.clear();
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

	// draws the bounding box of the quadtree
	instance->DrawRect(bounds->x, bounds->y, bounds->width, bounds->height);
}