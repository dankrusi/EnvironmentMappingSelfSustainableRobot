#include "HeatMapObject.h"

const double HeatMapObject::HEATSPOT_SIZE;
const double HeatMapObject::HEATSPOT_OPACITY;

HeatMapObject::HeatMapObject(Map *map, double x, double y, Core::AreaType type) : Object(map, x, y, HEATSPOT_SIZE, HEATSPOT_SIZE) 
{ 
	this->type = type; 
	this->strength = HEATSPOT_OPACITY; 
	this->width = HEATSPOT_SIZE;
	this->height = HEATSPOT_SIZE;
}

void HeatMapObject::paint(QPainter &painter, QRectF view)
{
	painter.setPen(Qt::transparent);
	painter.setOpacity(strength);
	if(type == Core::CollisionArea)		painter.setBrush(QBrush(Qt::red));
	else if(type == Core::OpenArea)		painter.setBrush(QBrush(Qt::green));
	painter.drawEllipse(QRectF(x - view.x() - (width/2), y - view.y() - (height/2), width, height));
}
