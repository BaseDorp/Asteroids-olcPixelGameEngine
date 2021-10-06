#include "Quadtree.h"

Quadtree::Quadtree(Rectangle* bounds)
{
	this->bIsSplit = false;
	this->bounds = bounds;
	this->maxObjects = 5;
}

void Quadtree::Split(std::vector<std::pair<SpaceObject*, SpaceObject*>> &collidingObjects)
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
				nodes[i]->Insert(o, collidingObjects);
			}
		}
	}

	this->objects.clear();
	this->bIsSplit = true;
}

void Quadtree::Insert(SpaceObject* spaceObject, std::vector<std::pair<SpaceObject*, SpaceObject*>> &collidingObjects)
{
	// if the object does not fit inside this quadtree, this isn't the right quadtree
	if (!bounds->Contains(spaceObject->x, spaceObject->y, spaceObject->size))
	{
		return;
	}

	if (!bIsSplit && this->objects.size() < maxObjects) // add the object to the quadtree if the max has not been hit
	{
		objects.push_back(spaceObject);

		for (auto o : this->objects)
		{
			// if the objects are not the same and overlapping (pythag)
			if (spaceObject != o &&
				((spaceObject->x - o->x) * (spaceObject->x - o->x)) + ((spaceObject->y - o->y) * (spaceObject->y - o->y)) <= (spaceObject->size + o->size) * (spaceObject->size + o->size))
			{
				// Add colliding pair to the collidingObjects vector
				collidingObjects.push_back(std::make_pair(spaceObject, o));
			}
		}

	}
	else
	{
		if (!this->bIsSplit) // splits the quadtree if this quadtree has not been split yet
		{
			this->Split(collidingObjects);
		}

		// adds the passed in object to one of the subnodes
		for (auto n : nodes)
		{
			n->Insert(spaceObject, collidingObjects);
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