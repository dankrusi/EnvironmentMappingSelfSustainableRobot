#ifndef PHYSICALMAP_H_
#define PHYSICALMAP_H_

#include <QImage>

#include "Map.h"

class PhysicalMap : public Map
{
	
public:
	QString 	filePath;
	QImage 		image;

public:
	PhysicalMap(QString filePath);
	virtual ~PhysicalMap();
	virtual void paint(QPainter &painter, QRectF view);
	Core::AreaType getAreaType(double x, double y);
	Core::AreaType checkForCollision(double x, double y, Object *exclude = NULL); 
};

#endif /*PHYSICALMAP_H_*/
