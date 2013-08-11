#ifndef GRIDMAP_H_
#define GRIDMAP_H_

#include "Map.h"

class GridMap : public Map
{
public:
	int gridSize;
	
public:
	GridMap(int gridSize);
	virtual ~GridMap();
	virtual void paint(QPainter &painter, QRectF view);
};

#endif /*GRIDMAP_H_*/
