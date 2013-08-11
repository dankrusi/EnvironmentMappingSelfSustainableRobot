/*
 * FadingCollisionMap.cpp
 *
 *  Created on: 22.03.2009
 *      Author: dgrob
 */

#include "FadingCollisionMap.h"

#include "../Library/Math.h"
#include "../Tracker/Tracker.h"
#include "../Core.h"
#include "../Controller/Controller.h"

FadingCollisionMap::FadingCollisionMap(Core *core) : Map("Fading Collision", core)
{
	collisions = new QList<FadingCollision>();
}

FadingCollisionMap::~FadingCollisionMap()
{
	delete collisions;
}

void FadingCollisionMap::registerObjectDetected(double distance, double angle, double opacity, int size) {

	// Create new collision
	FadingCollision newCollision;
	newCollision.position = (core->tracker->getTransformation() * Trafo2D::rot(Rad(angle)) * Trafo2D::trans(0, distance)).trans();
	newCollision.radius = size/2;
	newCollision.time.restart();

	fadeOldCollisions();
	addCollision(newCollision);
}

void FadingCollisionMap::connectController(QObject *target) {
	connect(target, SIGNAL(signalObjectDetected(double,double,double,int)), this, SLOT(registerObjectDetected(double,double,double,int)));
}

void FadingCollisionMap::paint(QPainter &painter, QRect view) {

	fadeOldCollisions(); //TODO: is this not performant enough?

	// Translate to our drawing system
	painter.translate(-view.x(), view.height() + view.y());
	QPen pen(Qt::red);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setBrush(Qt::transparent);

	lock.lockForRead(); {
		for(int i = 0; i < collisions->count(); i++){
			FadingCollision collision = collisions->at(i);
			painter.drawEllipse(QPoint(core->mmToPixels(collision.position.x()), -core->mmToPixels(collision.position.y())), (int)core->mmToPixels(collision.radius), (int)core->mmToPixels(collision.radius));
		}
	} lock.unlock();
}

void FadingCollisionMap::addCollision(FadingCollision collision) {
	// Append if not already a similar exists...
	bool foundSimilar = false;
	lock.lockForWrite(); {
		for(int i = 0; i < collisions->count(); i++) {
			if(collision.position == collisions->at(i).position) {
				foundSimilar = true;
				break;
			}
		}
		if(!foundSimilar) {
			collisions->append(collision);
		}
	} lock.unlock();
}


void FadingCollisionMap::fadeOldCollisions() {
	lock.lockForWrite(); {
		while(collisions->count() > 0) {
			FadingCollision collision = collisions->at(0);
			if(collision.time.elapsed() > 15000) {
				collisions->pop_front();
			} else {
				break;
			}
		}
	} lock.unlock();
}

bool FadingCollisionMap::isPointWithinCollisionArea(Vector2D point) {
	// Check each collision if point is within the circle boundry
	// by taking the distance from the center and seeing if it is
	// smaller than the radius...
	lock.lockForRead(); {
		for(int i = 0; i < collisions->count(); i++) {
			FadingCollision collision = collisions->at(i);
			if(dist(point, collision.position) < collision.radius) {
				lock.unlock();
				return true;
			}
		}
	} lock.unlock();
	return false;
}

long FadingCollisionMap::width() {
	return 0;
}

long FadingCollisionMap::height() {
	return 0;
}
