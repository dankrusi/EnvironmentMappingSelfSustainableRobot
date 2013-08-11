#include "../map/Map.h"

#include "../object/Object.h"

Map::Map()
{
	this->objects = new QList<Object*>();
}

Map::~Map()
{
	delete objects;
}

void Map::paintObjects(QPainter &painter, QRectF view)
{
	// Loop all objects
	for(int i = 0; i < objects->count(); i++){
		// Check bounding box of object
		Object *obj = objects->at(i);
		if(
				obj->x + obj->width >= view.x() &&
				obj->x - obj->width <= view.x()+view.width() &&
				obj->y + obj->height >= view.y() &&
				obj->y - obj->height <= view.y()+view.height()
		) {
			Core::resetPainter(painter);
			obj->paint(painter, view);
		}
	}
}

Core::AreaType Map::checkForCollision(double x, double y, Object *exclude)
{
	for(int i = 0; i < objects->count(); i++) {
		if(objects->at(i) != exclude && objects->at(i)->checkForCollision(x,y) == true) return Core::CollisionArea;
	}
	return Core::OpenArea;
}

