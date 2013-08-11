#ifndef PERSONOBJECT_H_
#define PERSONOBJECT_H_

#include "Object.h"

class PersonObject : public Object
{
	Q_OBJECT
	
public:
	static const double BODY_WIDTH = 50.0;
	static const double BODY_HEIGHT = 20.0;
	static const double HEAD_WIDTH = 30.0;
	static const double HEAD_HEIGHT = 30.0;
	static const double FOOT_WIDTH = 10.0;
	static const double FOOT_HEIGHT = 20.0;
	
public:
	PersonObject(Map *map, double x, double y) : Object(map, x, y, BODY_WIDTH, HEAD_HEIGHT) {};
	virtual ~PersonObject() {};
	virtual void paint(QPainter &painter, QRectF view);
};

#endif /*PERSONOBJECT_H_*/
