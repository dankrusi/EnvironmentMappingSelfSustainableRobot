#ifndef HEATMAPOBJECT_H_
#define HEATMAPOBJECT_H_

#include "Object.h"

class HeatMapObject : public Object
{
	
public:
	static const double HEATSPOT_SIZE = 50.0;
	static const double HEATSPOT_OPACITY = 0.5;
	
public:
	Core::AreaType 		type;
	double				strength;
	
public:
	HeatMapObject(Map *map, double x, double y, Core::AreaType type);
	virtual ~HeatMapObject() {};
	virtual void paint(QPainter &painter, QRectF view);
};

#endif /*HEATMAPOBJECT_H_*/
