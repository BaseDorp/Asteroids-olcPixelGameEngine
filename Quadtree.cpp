#include "Quadtree.h"

//Quadtree::Quadtree()
//{
//	this->maxLevels = 4;
//	this->bIsSplit = false;
//}

Quadtree::Quadtree(Rectangle* bounds)
{
	this->maxLevels = 4;
	this->bIsSplit = false;
	this->bounds = bounds;
	this->level = 0;
	this->maxObjects = 4;

	this->topLeft = nullptr;
	this->topRight = nullptr;
	this->bottomLeft = nullptr;
	this->bottomRight = nullptr;
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

	/*nodes[0] = new Quadtree(level + 1, new Rectangle(x + subWidth, y, subWidth, subHeight));
	nodes[1] = new Quadtree(level + 1, new Rectangle(x, y, subWidth, subHeight));
	nodes[2] = new Quadtree(level + 1, new Rectangle(x, y + subHeight, subWidth, subHeight));
	nodes[3] = new Quadtree(level + 1, new Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));*/

	/*topLeft = new Quadtree(Rectangle(x, y, subWidth, subHeight));
	topRight = new Quadtree(Rectangle(x + subWidth, y, subWidth, subHeight));
	bottomLeft = new Quadtree(Rectangle(x, y + subHeight, subWidth, subHeight));
	bottomRight = new Quadtree(Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));*/

	for (auto o : this->objects)
	{
			for (int i = 0; i < nodes.size(); i++)
			{
				if (nodes[i]->bounds->Contains(o->x, o->y))
				{
					nodes[i]->Insert(o);
				}
			}

		/*if (nodes[0]->bounds.Contains(o->x, o->y))
		{
			nodes[0]->Insert(o);
		}
		else if (nodes[1]->bounds.Contains(o->x, o->y))
		{
			nodes[1]->Insert(o);
		}
		else if (nodes[2]->bounds.Contains(o->x, o->y))
		{
			nodes[2]->Insert(o);
		}
		else if (nodes[3]->bounds.Contains(o->x, o->y))
		{
			nodes[3]->Insert(o);
		}*/
	}

	//this->objects.clear();
	this->bIsSplit = true;
}

int Quadtree::GetIndex()
{
	/*int index = -1;
	float verticalMidpoint = x + (width / 2);
	float horizontalMidpoint = y + (height / 2);

	bool topQuadtrant = (y < horizontalMidpoint&& y + height < horizontalMidpoint);
	bool bottomQuadrant = (y > horizontalMidpoint);*/

	//	// Object can completely fit within the top quadrants
	//	boolean topQuadrant = (pRect.getY() < horizontalMidpoint && pRect.getY() + pRect.getHeight() < horizontalMidpoint);
	//// Object can completely fit within the bottom quadrants
	//boolean bottomQuadrant = (pRect.getY() > horizontalMidpoint);
	//if (pRect.getX() < verticalMidpoint && pRect.getX() + pRect.getWidth() < verticalMidpoint) {
	//	if (topQuadrant) {
	//		index = 1;
	//	}
	//	else if (bottomQuadrant) {
	//		index = 2;
	//	}
	//}
	//// Object can completely fit within the right quadrants
	//else if (pRect.getX() > verticalMidpoint) {
	//	if (topQuadrant) {
	//		index = 0;
	//	}
	//	else if (bottomQuadrant) {
	//		index = 3;
	//	}
	//}


	/*if (x < verticalMidpoint && x + width < verticalMidpoint)
	{
		if (topQuadtrant)
		{
			index = 1;
		}
		else if (bottomQuadrant)
		{
			index = 2;
		}
	}
	else if (x > verticalMidpoint)
	{
		if (topQuadtrant)
		{
			index = 0;
		}
		else if (bottomQuadrant)
		{
			index = 3;
		}
	}

	return index;*/
	return 0;
}

void Quadtree::Insert(SpaceObject* spaceObject)
{
	// check if the object is inside this quadtree // for recurssion
	if (!bounds->Contains(spaceObject->x, spaceObject->y))
	{
		return;
	}

	// add the object to the quadtree if the max amount of objects has not been reached
	if (this->objects.size() < maxObjects) 
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
		/*this->topLeft->Insert(spaceObject);
		this->topRight->Insert(spaceObject);
		this->bottomLeft->Insert(spaceObject);
		this->bottomRight->Insert(spaceObject);*/
	}

	/*public void insert(Rectangle pRect) {
		if (nodes[0] != null) {
			int index = getIndex(pRect);

			if (index != -1) {
				nodes[index].insert(pRect);

				return;
			}
		}

		objects.add(pRect);

		if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
			if (nodes[0] == null) {
				split();
			}

			int i = 0;
			while (i < objects.size()) {
				int index = getIndex(objects.get(i));
				if (index != -1) {
					nodes[index].insert(objects.remove(i));
				}
				else {
					i++;
				}
			}
		}
	}*/

	//if (nodes[0] != nullptr)
	//{
	//	int index = GetIndex(/*rectangle*/);

	//	if (index != -1)
	//	{
	//		//nodes[index]->Insert(/*rectangle*/);
	//		return;
	//	}
	//}

	//objects.push_back(/*rectangle*/);

	//if (objects.size() > maxObjects && level < maxLevels)
	//{
	//	if (nodes[0] == nullptr)
	//	{
	//		Split();
	//	}

	//	int i = 0;
	//	while (i < objects.size())
	//	{
	//		/*int index = GetIndex(objects.get(i));
	//		if (index != -1)
	//		{
	//			nodes[index]->Insert(objects.remove(i));
	//		}
	//		else
	//		{
	//			i++;
	//		}*/
	//	}
	//}

}

void Quadtree::Clear()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		objects.clear(); // Clear the objects in this quadtree

		if (nodes[i] != nullptr)
		{
			// Go into the nodes of this quadtree and call that clear function
			nodes[i]->Clear(); 
			nodes[i] = nullptr;
		}
	}
}

std::vector<SpaceObject*> Quadtree::Retrieve(std::vector<SpaceObject*> returnObjects, Rectangle rectangle)
{
	int index = GetIndex(/*rectangle*/);
	if (index != -1 && nodes[0] != nullptr)
	{
		nodes[index]->Retrieve(returnObjects, rectangle);
	}

	//returnObjects.addAll(objects);

	return std::vector<SpaceObject*>();
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

		/*if (topLeft != nullptr && objects.size() > 0)
		{
			topLeft->Draw(instance);
		}
		if (topRight != nullptr && objects.size() > 0)
		{
			topRight->Draw(instance);
		}
		if (bottomLeft != nullptr && objects.size() > 0)
		{
			bottomLeft->Draw(instance);
		}
		if (bottomRight != nullptr && objects.size() > 0)
		{
			bottomRight->Draw(instance);
		}*/
	}
}