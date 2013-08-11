/*
 *  RawMovementTracker.cpp
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

#include "RawMovementTracker.h"

#include "../Map/ColorMap.h"
#include "../Map/HeatMap.h"

#include "../Library/Debug.h"

RawMovementTracker::RawMovementTracker(Core *core, long x, long y, double rotation) : MovementTracker("Raw", core) {
	transformation = Trafo2D::trans(x,y) * Trafo2D::rot(Rad(rotation)); // Core/COIL has a clockwise positive rotation while the math library has a anti-clockwise rotation...
	weight = 1;
}

RawMovementTracker::~RawMovementTracker() {

}

void RawMovementTracker::registerMovedDistance(double dist){

	if(dist != 0) {

		lock.lockForWrite(); {

			relativeDistance += dist;
			totalDistance += std::abs(dist);
			transformation = transformation * Trafo2D::trans(0, dist); // Move forwards (up/y) in our transformation matrix

		} lock.unlock();

		emit moved(this->x(), this->y(), this->rotation());
	}



}

void RawMovementTracker::registerChangedAngle(double angle){

	if(angle != 0) {

		lock.lockForWrite(); {

			relativeAngle += angle;
			totalAngle += std::abs(angle);
			transformation = transformation * Trafo2D::rot(Rad(angle)); // Core/COIL has a clockwise positive rotation while the math library has a anti-clockwise rotation...

		} lock.unlock();

		emit moved(this->x(), this->y(), this->rotation());
	}



}

void RawMovementTracker::registerChangedWheelSpeed(int, int){
	// No implementation needed
}



