/*
 *  AccelerationFilterWeight.h
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

#ifndef ACCELERATIONFILTERWEIGHT_H_
#define ACCELERATIONFILTERWEIGHT_H_

#include "Weight.h"

class AccelerationFilterWeight : public Weight {

	Q_OBJECT;

private:
	Vector2D vOld;

public:
	AccelerationFilterWeight(Core *core) : Weight("Acceleration Filter", core) {
		vOld = Vector2D(0.0,0.0);
	};

	virtual ~AccelerationFilterWeight() {};

	virtual void process(Vector2D &v) {
		double f = 0.25; // The maximum amount of change allowed in one process (based on max of 1.0)
		Vector2D dv = v - vOld;
		if(dv.x() > f || dv.x() < -f) dv = Vector2D(dv.x()*f,dv.y());
		if(dv.y() > f || dv.y() < -f) dv = Vector2D(dv.x(),dv.y()*f);
		v = vOld + dv;
		vOld = v;
	};
};

#endif /* ACCELERATIONFILTERWEIGHT_H_ */
