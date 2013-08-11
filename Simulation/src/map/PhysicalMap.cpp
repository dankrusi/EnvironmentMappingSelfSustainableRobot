#include "PhysicalMap.h"

PhysicalMap::PhysicalMap(QString filePath) : Map()
{
	this->filePath = filePath;
	this->image = QImage(filePath);
	this->width = this->image.width();
	this->height = this->image.height();
}

PhysicalMap::~PhysicalMap()
{
}

void PhysicalMap::paint(QPainter &painter, QRectF view) {
	// Draw background map
	double width = view.width();
	double height = view.height();
	double x = view.x();
	double y = view.y();
	if(x + width > this->image.width()) width = this->image.width() - x;
	if(y + height > this->image.height()) height = this->image.height() - y;
	QRectF target = QRectF(0, 0, width, height);
	QRectF source = QRectF(x, y, width, height);
	painter.drawImage(target, image, source);
	
	// Paint all the objects...
	paintObjects(painter, view);
}

Core::AreaType PhysicalMap::getAreaType(double x, double y) {
	QRgb pixel = image.pixel((int)x, (int)y); 
	if(pixel == qRgb(255,0,0)) 			return Core::CollisionArea;
	else if(pixel == qRgb(0,255,0)) 	return Core::OpenArea;
	else 								return Core::UnknownArea;
}

Core::AreaType PhysicalMap::checkForCollision(double x, double y, Object *exclude) {
	for(int i = 0; i < objects->count(); i++) {
		if(objects->at(i) != exclude && objects->at(i)->checkForCollision(x,y) == true) return Core::CollisionArea;
	}
	return getAreaType(x,y);
}
