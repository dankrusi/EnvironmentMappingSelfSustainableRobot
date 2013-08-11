/*
 * MarkerObject.h
 *
 *  Created on: 24.02.2009
 *      Author: dkruesi
 */

#ifndef MARKEROBJECT_H_
#define MARKEROBJECT_H_

#include "Object.h"

#include "../Library/Math.h"

class MarkerObject : public Object
{
	Q_OBJECT

private:
	int transformationCount;
	Trafo2D transformation[6];
	QColor transformationColors[6];
	long movementScale;


public:
	MarkerObject(ObjectMap *map, long movementScale) : Object(map, "Marker", 0, 0, 0, 0) { transformationCount = 0; this->movementScale = movementScale; };
	virtual ~MarkerObject() {};
	virtual void paint(QPainter &painter, QRect view, int scale);
	void setAsVector(int index, Vector2D vector);
	void setAsPoint(int index, long x, long y);
	void setAsTransformation(int index, Trafo2D transformation, QColor color);
};

#endif /* MARKEROBJECT_H_ */
