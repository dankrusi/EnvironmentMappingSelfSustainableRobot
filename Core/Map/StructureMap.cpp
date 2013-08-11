/*
 * StructureMap.cpp
 *
 *  Created on: 22.03.2009
 *      Author: dgrob
 */

#include "StructureMap.h"

#include "../Library/Math.h"
#include "../Tracker/Tracker.h"
#include "../Core.h"
#include "../Controller/Controller.h"

StructureMap::StructureMap(Core *core) : Map("Structure", core)
{
	finish = false;
}

StructureMap::~StructureMap()
{
}

void StructureMap::registerObjectDetected(double distance, double angle, double opacity, int size) {
	if (finish) return;
	Trafo2D objectLocation = core->tracker->getTransformation() * Trafo2D::rot(Rad(angle)) * Trafo2D::trans(0, distance); // Core/COIL has a clockwise positive rotation while the math library has a anti-clockwise rotation...

	double distanceBetweenStructurePoints = core->doubleSetting("Map_StructureMap_MinimumDistanceBetweenCollision");
	double finishTolerance = core->doubleSetting("Map_StructureMap_FinishTolerance_mm");

	Vector2D p = Vector2D(objectLocation.trans().x(),objectLocation.trans().y());

	lock.lockForWrite();
	{
		if (collisions.count() <= 0){
			this->collisions.append(p);
		}
		else if(dist(p, collisions.last()) > distanceBetweenStructurePoints && collisions.count() > 0){
			this->collisions.append(p);
		}
		if (collisions.count() > 1 && dist(collisions.first(), collisions.last()) < finishTolerance){

			// Sort the coordinates
			for (int i = 0; i < collisions.count(); i++){
				int distance = INT_MAX;
				int nearestPoint = 0;
				for(int j = i+1; j < collisions.count(); j++){
					int temp = dist(collisions[i], collisions[j]);
					if (temp < distance){
						distance = temp;
						nearestPoint = j;
					}
				}
				if (i != collisions.count() - 1) {
					Vector2D temp = collisions[i + 1];
					collisions[i + 1] = collisions[nearestPoint];
					collisions[nearestPoint] = temp;
				}

			}

			finish = true;
		}
	}
	lock.unlock();


}

void StructureMap::paint(QPainter &painter, QRect view) {

	// Translate to our drawing system
	painter.translate(-view.x(), view.height() + view.y());
	QPen pen(Qt::yellow);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setBrush(Qt::transparent);

	lock.lockForRead(); {
		for(int i = 0; i < collisions.count()-1; i++){
			int x1 = (int) collisions.at(i).x();
			int y1 = (int) collisions.at(i).y();
			int x2 = (int) collisions.at(i+1).x();
			int y2 = (int) collisions.at(i+1).y();
			painter.drawLine(
					core->mmToPixels(x1),
					-core->mmToPixels(y1),
					core->mmToPixels(x2),
					-core->mmToPixels(y2)
					);
		}
		if(finish){
			painter.drawLine(
					core->mmToPixels(collisions.first().x()),
					-core->mmToPixels(collisions.first().y()),
					core->mmToPixels(collisions.last().x()),
					-core->mmToPixels(collisions.last().y())
					);
		}
	} lock.unlock();

}
void  StructureMap::connectController(QObject *target) {
	connect(target, SIGNAL(signalObjectDetected(double,double,double,int)), this, SLOT(registerObjectDetected(double,double,double,int)));
}

bool StructureMap::isFinish(){
	return finish;
}

bool StructureMap::isPointOutside(Vector2D p){

	// Algorithm via http://sidvind.com/wiki/Point-in-polygon:_Jordan_Curve_Theorem

	if (!finish) return false;
	double x1, x2;
	int crossings = 0;
	double eps = 0.000001;

	for(int i = 0; i < collisions.count(); i++){
		if ( collisions.at(i).x() < collisions.at( (i+1) % collisions.count() ).x() ){
			x1 = collisions.at(i).x();
			x2 = collisions.at( (i+1) % collisions.count() ).x();
		}
		else {
			x1 = collisions.at( (i+1) % collisions.count() ).x();
			x2 = collisions.at(i).x();
		}

		if (p.x() > x1 && p.x() <= x2 && ( p.y() < collisions.at(i).y() || p.y() < collisions.at( (i+1)%collisions.count() ).y() )){
			double dx = collisions.at( (i+1)%collisions.count() ).x() - collisions.at(i).x();
			double dy = collisions.at( (i+1)%collisions.count() ).y() - collisions.at(i).y();
			double k = 0;

			if ( std::abs(dx) < eps ){
				k = INT_MAX;
			}
			else {
				k = dy/dx;
			}

			double m = collisions.at(i).y() - (k * collisions.at(i).x());

			double y2 = k * p.x() + m;
			if (p.y() <= y2){
				crossings++;
			}
		}
	}
	if (crossings %2 == 1){
		// Point p is inside the polygon
		return false;
	}
	return true;
}

long StructureMap::width() {
	return 0;
}

long StructureMap::height() {
	return 0;
}


