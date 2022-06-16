#include "SpaceObject.h"

//SpaceObject::SpaceObject()
//{
//	this->x = 0.0f;
//	this->y = 0.0f;
//	this->vx = 0.0f;
//	this->vy = 0.0f;
//	this->size = 1.0f;
//	this->angle = 0.0f;
//}

SpaceObject::SpaceObject(float x, float y, float vx, float vy, int size, float angle)
{
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	this->size = size;
	this->angle = angle;
	this->ax = 0;
	this->ay = 0;
}

//SpaceObject::SpaceObject(float x = 0.0f, float y = 0.0f, float vx = 0.0f, float vy = 0.0f, int size = 0.0f, float angle = 0.0f)
//{
//	this->x = x;
//	this->y = y;
//	this->vx = vx;
//	this->vy = vy;
//	this->size = size;
//	this->angle = angle;
//}