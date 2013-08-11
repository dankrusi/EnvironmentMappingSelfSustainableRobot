#include "../object/Object.h"

#include "../map/Map.h"

Object::Object(Map *map, double x, double y, double width, double height) { 
	// Init
	this->map = map;
	this->x = x; 
	this->y = y; 
	this->width = width; 
	this->height = height; 
	this->rotation = 0.0;
	
	// Register with map
	if(map != NULL) this->map->objects->append(this);
};

bool Object::checkForCollision(double x, double y) 
{
	if(		x >= this->x - (width/2) &&
			x <= this->x + (width/2) &&
			y >= this->y - (height/2) &&
			y <= this->y + (height/2)	) 	return true;
	else 									return false;
}

