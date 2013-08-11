/*
 * StructureMap.h
 *
 *  Created on: 22.03.2009
 *      Author: dgrob
 */

#ifndef STRUCTUREMAP_H_
#define STRUCTUREMAP_H_

#include <QPoint>
#include <QList>

#include "../Library/Math.h"

#include "Map.h"


class StructureMap : public Map{
	Q_OBJECT

private:
	QList<Vector2D> collisions;
	bool finish;

public:
	StructureMap(Core *core);
	virtual ~StructureMap();
	void connectController(QObject *target);
	bool isFinish();
	bool isPointOutside(Vector2D p);

public:
	virtual void paint(QPainter &painter, QRect view);
	virtual long width();
	virtual long height();

public slots:
	virtual void registerObjectDetected(double, double, double, int);

};

#endif /* STRUCTUREMAP_H_ */
