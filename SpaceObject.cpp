#include "SpaceObject.h"

SpaceObject::SpaceObject()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->dx = 0.0f;
	this->dy = 0.0f;
	this->size = 1.0f;
	this->angle = 0.0f;
}

SpaceObject::SpaceObject(float x = 0.0f, float y = 0.0f, float dx = 0.0f, float dy = 0.0f, int size = 1.0f, float angle = 0.0f)
{
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	this->size = size;
	this->angle = angle;
}