#ifndef HEATMAP_H_
#define HEATMAP_H_

#include <QImage>

#include "Map.h"

class HeatMap : public Map
{
	
public:
	static const double 			HEATSPOT_SIZE = 40.0;
	static const double 			HEATSPOT_OPACITY = 0.2;
	static const Qt::GlobalColor	COLLISIONAREA_COLOR = Qt::red;
	static const Qt::GlobalColor	OPENAREA_COLOR = Qt::green;
	
public:
	QImage 	*heat;
	bool	antiAlias;
	
public:
	HeatMap(double width, double height);
	virtual ~HeatMap();
	virtual void paint(QPainter &painter, QRectF view);
	void registerHeat(double x, double y, Core::AreaType type, double opacity = HEATSPOT_OPACITY, double size = HEATSPOT_SIZE);
	int getCollisionAreaHeat(double x, double y);
};

#endif /*HEATMAP_H_*/
