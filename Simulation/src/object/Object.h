#ifndef OBJECT_H_
#define OBJECT_H_

#include <QObject>
#include <QPainter>
#include <QString>

#include "../core/Core.h"

class Map;
//#include "../map/Map.h"

class Object : public QObject
{
	Q_OBJECT
	
public:
	double width;
	double height;
	double x;
	double y;
	double rotation;
	Map		*map;
	
private:
	
	
public:
	Object(Map *map, double x, double y, double width, double height);
	virtual ~Object() {};
	virtual void paint(QPainter &painter, QRectF view) = 0;
	virtual bool checkForCollision(double x, double y);
	
};

#endif /*OBJECT_H_*/
