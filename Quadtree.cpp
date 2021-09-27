#include "Quadtree.h"

Quadtree::Quadtree()
{
	this->maxLevels = 4;
	this->bIsSplit = false;
}

Quadtree::Quadtree(Rectangle bounds)
{
	this->maxLevels = 4;
	this->bIsSplit = false;
}

void Quadtree::Split()
{
	float x = bounds->x;
	float y = bounds->y;
	float subWidth = bounds->width / 2;
	float subHeight = bounds->height / 2;

	/*nodes[0] = new Quadtree(level + 1, new Rectangle(x + subWidth, y, subWidth, subHeight));
	nodes[1] = new Quadtree(level + 1, new Rectangle(x, y, subWidth, subHeight));
	nodes[2] = new Quadtree(level + 1, new Rectangle(x, y + subHeight, subWidth, subHeight));
	nodes[3] = new Quadtree(level + 1, new Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));*/

	topLeft = new Quadtree(Rectangle(x, y, subWidth, subHeight));
	topRight = new Quadtree(Rectangle(x + subWidth, y, subWidth, subHeight));
	bottomLeft = new Quadtree(Rectangle(x, y + subHeight, subWidth, subHeight));
	bottomRight = new Quadtree(Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));

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
	if (bounds->Contains(spaceObject->x, spaceObject->y))
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

		topLeft->Insert(spaceObject);
		topRight->Insert(spaceObject);
		bottomLeft->Insert(spaceObject);
		bottomRight->Insert(spaceObject);
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
