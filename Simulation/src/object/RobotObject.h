#ifndef ROBOTOBJECT_H_
#define ROBOTOBJECT_H_

#include "Object.h"

class RobotObject : public Object 
{
	Q_OBJECT
	
public:
	RobotObject(Map *map, double x, double y) : Object(map, x, y, 50.0, 50.0) {};
	virtual ~RobotObject() {};
	virtual void paint(QPainter &painter, QRectF view);
};

#endif /*ROBOTOBJECT_H_*/
