/*
 * FadingCollisionMap.h
 *
 *  Created on: 22.03.2009
 *      Author: dgrob
 */

#ifndef FADINGCOLLISIONMAP_H_
#define FADINGCOLLISIONMAP_H_

#include <QList>
#include <QMutex>

#include "Map.h"

#include "../Library/Math.h"

struct FadingCollision {
	Vector2D position;
	double radius;
	QTime time;
};

class FadingCollisionMap : public Map {

	Q_OBJECT

private:
	QList<FadingCollision> *collisions;
	QMutex _mutex;

public:
	FadingCollisionMap(Core *core);
	~FadingCollisionMap();
	void connectController(QObject *target);
	void fadeOldCollisions();
	void addCollision(FadingCollision collision);
	bool isPointWithinCollisionArea(Vector2D point);

public:
	virtual void paint(QPainter &painter, QRect view);
	virtual long width();
	virtual long height();

public slots:
	void registerObjectDetected(double distance, double angle, double opacity, int size);

};

#endif /* FADINGCOLLISIONMAP_H_ */
