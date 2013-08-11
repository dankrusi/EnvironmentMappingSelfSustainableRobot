/*
 *  OrientationWeight.h
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

#ifndef ORIENTATIONWEIGHT_H_
#define ORIENTATIONWEIGHT_H_

#include "Weight.h"

#include "../../Tracker/Tracker.h"
#include "../../Navigation/Navigation.h"
#include "../../Object/MarkerObject.h"
#include "../../Library/Debug.h"

class OrientationWeight : public Weight {

	Q_OBJECT;

public:
	OrientationWeight(Core *core) : Weight("Orientation", core) {};

	virtual ~OrientationWeight() {};

	virtual void process(Vector2D &v) {
		Vector2D position = core->tracker->getTranslation();
		Vector2D destination = core->navigation->getCurrentDestination();
		double alpha = -Deg(angle(position,destination)) - core->tracker->getRotation();
		if(alpha > 180.0) alpha = -360.0 + alpha;
		if(alpha < -180.0) alpha = 360.0 + alpha;
		v = v + Vector2D( (-alpha/180.0)/1.0 , (+alpha/180.0)/1.0 );
	};
};

#endif /* ORIENTATIONWEIGHT_H_ */
