/*
 *  ExpectedMovementTracker.h
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ExpectedMovementTracker.h"

#include "../Library/Debug.h"

#include "../Map/HeatMap.h"

ExpectedMovementTracker::ExpectedMovementTracker(Core *core, long x, long y, double rotation) : MovementTracker("Expected", core) {
	transformation = Trafo2D::trans(x,y) * Trafo2D::rot(Rad(rotation)); // Core/COIL has a clockwise positive rotation while the math library has a anti-clockwise rotation...
	lastGetDistanceTimestamp = QTime();
	lastGetAngleTimestamp = QTime();
	wheelSpeed = Vector2D(0,0);
	weight = 1;
}

ExpectedMovementTracker::~ExpectedMovementTracker() {

}


void ExpectedMovementTracker::registerChangedWheelSpeed(int x, int y){

	lock.lockForWrite(); {

		// Get the elapsed time since last call
		double elapsedTimeSinceGetDistance = (double)lastGetDistanceTimestamp.restart();

		// We need to return a value (mm) based on the wheel speed (mm/s) and elapsed time (ms)...
		double speed = ((double)wheelSpeed.x() + (double)wheelSpeed.y()) / 2.0; // Average it out
		double distance = (speed * (elapsedTimeSinceGetDistance / 1000.0));


		// Get the elapsed time since last call
		double elapsedTimeSinceGetAngle = (double)lastGetAngleTimestamp.restart();
		double angle = 0.0;

		// Calculate change of angle using physics per [KrusiGrob08] pg 36...

		if (wheelSpeed.x() != wheelSpeed.y()) {
			double d = core->doubleSetting("Robot_WheelOffset_mm") * 2;
			if(wheelSpeed.x() != 0) {
				// Based the radius of the circular motion on the left wheel. If emulatedLwheel is zero,
				// then we cannot do this because it is not a circle and the change in angle is infinately large...
				double arclenL = wheelSpeed.x() * (elapsedTimeSinceGetAngle / 1000.0);
				double radiusL = (wheelSpeed.x()*d) / (wheelSpeed.y()-wheelSpeed.x());
				angle = arclenL / radiusL;
			} else {
				// If we are here, then emulatedLwheel is zero so we simplify angleAlpgha accordingly
				double arclenR = wheelSpeed.y() * (elapsedTimeSinceGetAngle / 1000.0);
				angle = arclenR / d;
			}
		}

		if(angle != 0 || distance != 0) {
			transformation = transformation * Trafo2D::trans(0, distance);
			transformation = transformation * Trafo2D::rot(angle);

			// Register total/relative dist/angle...
			relativeDistance += distance;
			totalDistance += std::abs(distance);
			relativeAngle += angle;
			totalAngle += std::abs(angle);


		}

		wheelSpeed = Vector2D(x,y);

	} lock.unlock();

	emit moved(this->x(), this->y(), this->rotation());
}

void ExpectedMovementTracker::registerMovedDistance(double dist){
	// No implementation needed
}

void ExpectedMovementTracker::registerChangedAngle(double angle){
	// No implementation needed
}




