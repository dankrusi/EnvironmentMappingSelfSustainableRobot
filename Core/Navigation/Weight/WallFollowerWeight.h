/*
 *  WallFollowerWeight.h
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

#ifndef WALLFOLLOWERWEIGHT_H_
#define WALLFOLLOWERWEIGHT_H_

#include "Weight.h"

#include "../../COIL/COIL.h"
#include "../../Controller/Controller.h"
#include "../Navigation.h"

#include "../../Task/WallFollowerTask.h"
#include "../../Task/TaskManager.h"
#include "../../Tracker/Tracker.h"

#include "../../Library/Math.h"

#include "../../Library/Debug.h"


class WallFollowerWeight : public Weight {

	Q_OBJECT;

private:
	bool wallDetected;
	Vector2D robotPositionByWallDetected;
	double initialRotation;
	double fx;
	QTime lastWallDetection;

public:
	WallFollowerWeight(Core *core) : Weight("Wall Follower", core) {
		wallDetected = false;
	};

	virtual ~WallFollowerWeight() {};

	virtual void process(Vector2D &v) {
		double wallDistance = (double) core->coil->getWallSensorDistanceFromSignal(core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_WALL_SIGNAL));
		if(!wallDetected && wallDistance < core->doubleSetting("Robot_WallSensorRange_mm")) {
			wallDetected = true;
			fx = 0.8;
			lastWallDetection = QTime::currentTime();
			Debug::print("[WallFollowerWeight] Wall detected");
		}

		if (wallDetected){
			// Correct back to wall - turn softly left
			if(wallDistance < core->doubleSetting("Robot_WallSensorRange_mm")) {
				fx = 0.8;
				lastWallDetection = QTime::currentTime();
				v = Vector2D(v.x() * fx , v.y());
			} else {
				if(QTime::currentTime().second() - lastWallDetection.second() > 5 && fx > 0){
					fx -= 0.1;
				}
				v = Vector2D(v.x() , v.y() * fx);
			}
		}
		else {
			// go straight forward
			v = Vector2D(v.x() , v.y());
		}





	};
};


#endif /* WALLFOLLOWERWEIGHT_H_ */
