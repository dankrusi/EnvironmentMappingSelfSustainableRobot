/*
 *  Navigation.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework  is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Navigation.h"

#include "../Core.h"
#include "../Library/Debug.h"
#include "../Tracker/Tracker.h"
#include "../Map/HeatMap.h"
#include "../Map/FadingCollisionMap.h"

#include <limits.h>

Navigation::Navigation(QString name, Core *core) : CoreObject(name, core){

	navPoints = new QList<Vector2D> ();
	navPath = new QList<Vector2D> ();
	currentNavPoint = 0;

	Debug::print("[Navigation] %1 Navigation loaded", name);
}

Navigation::~Navigation() {

	delete navPoints;
	delete navPath;

}

// Wrapper method which checks to see if we need to add some points before
// or after in order to create a valid spline...
void Navigation::addNavPoint(Vector2D point) {

	// Set start point
	if (navPoints->count() == 0) {

		// Add starting point
		Vector2D startPoint(Vector2D(this->core->tracker->getX(), this->core->tracker->getY()));
		processAddNavPoint(startPoint);

		// Add additional points for a spline start
		if(core->stringSetting("Navigation_AutomaticallyAddStartingNavPoints") == "true") {
			Vector2D splineStartPoint1 = this->core->tracker->getTransformation() * Vector2D(0, 400);
			Vector2D splineStartPoint2 = this->core->tracker->getTransformation() * Vector2D(0, 800);
			processAddNavPoint(splineStartPoint1);
			processAddNavPoint(splineStartPoint2);
		}
	}

	// Call the real method to add the point
	processAddNavPoint(point);
}

void Navigation::addNavPoint(long x, long y) {
	addNavPoint(Vector2D((double) x, (double) y)); // Forward to master function
}

void Navigation::processAddNavPoint(Vector2D point) {
	navPoints->append(point);
}

void Navigation::registerPointOnNavPath(Vector2D point) {
	navPath->append(point);
}

void Navigation::registerPointOnNavPath(long x, long y) {
	registerPointOnNavPath(Vector2D((double) x, (double) y)); // Forward to master function
}

QList<Vector2D>* Navigation::getNavPoints() {
	return navPoints;
}

QList<Vector2D>* Navigation::getNavPath() {
	return navPath;
}

void Navigation::clearNavPoints() {
	navPoints->clear();
	currentNavPoint = 0;
}

int Navigation::getCurrentNavPoint() {
	return currentNavPoint;
}

bool Navigation::isAtLastNavPoint() {
	return ( getCurrentNavPoint() >= navPoints->count()-1 );
}

Vector2D Navigation::getCurrentDestination() {
	if(navPoints->count() == 1)							return navPoints->at(0);
	else if(getCurrentNavPoint() < navPoints->count() -1) 	return navPoints->at(getCurrentNavPoint()+1);
	else												return Vector2D(0,0); // Unknown location
}

// Returns the distance to a possible collision straight ahead or backwards using the heat map.
// Note: If no collision is within the map bounds, INT_MAX is returned.
long Navigation::getMaxOpenAreaDistanceFromMap(CollisionSource source, CollisionDirection direction){
	// Set max range to the longest possible range within map bounds
	//long maxRange = (long)sqrtf((float)core->mapWidth*(float)core->mapWidth + (float)core->mapHeight*(float)core->mapHeight);
	long maxRange = core->longSetting("Navigation_MaxOpenAreaDistanceMaxRange_mm");

	for(long i = 0; i < maxRange; i++) {

		// Set the direction and get the test position
		int dir = (direction == Navigation::CollisionDirectionAhead ? +1 : -1);
		Trafo2D position = core->tracker->getTransformation() * Trafo2D::trans(0, dir*i);

		// Check if out of map bounds...
		if(position.trans().x() < 0 || position.trans().y() < 0 || position.trans().x() > core->mapWidth || position.trans().y() > core->mapHeight) return INT_MAX;

		// Return if i if collision detected at position
		if(source == CollisionSourceHeatMap) {
			if (core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel,  (int)(position.trans().x()), (int)(position.trans().y())) > 0.0) return i;
		} else if (source == CollisionSourceFadingCollisionMap) {
			if (core->fadingCollisionMap->isPointWithinCollisionArea(position.trans())) return i;
		}
	}

	return INT_MAX;
}

// Returns the distance to the wall using the heat map. Is now wall inside the reach
// of the wall sensor INT_MAX will be returned.
int Navigation::getWallDistanceFromMap(CollisionSource source){
	Trafo2D position;
	for(int i = 0; i < core->intSetting("Robot_WallSensorRange_mm"); i++){
		position = core->tracker->getTransformation() * Trafo2D::rot(Rad(-90)) * Trafo2D::trans(0, (core->intSetting("Robot_Diameter_mm")/2) + i);
		if(source == CollisionSourceHeatMap) {
			if (core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel,  (int)(position.trans().x()), (int)(position.trans().y())) > 0.0) return i;
		} else if (source == CollisionSourceFadingCollisionMap) {
			if (core->fadingCollisionMap->isPointWithinCollisionArea(position.trans())) return i;
		}
	}
	return INT_MAX;
}

