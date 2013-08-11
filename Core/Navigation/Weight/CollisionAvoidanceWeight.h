/*
 *  CollisionAvoidanceWeight.h
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

#ifndef COLLISIONAVOIDANCEWEIGHT_H_
#define COLLISIONAVOIDANCEWEIGHT_H_

#include "Weight.h"

#include "../../Tracker/Tracker.h"

#include "../../Navigation/Navigation.h"

#include "../../Library/Debug.h"
#include "../../Library/Math.h"

#include "../../COIL/COIL.h"

#include "../../Controller/Controller.h"

class CollisionAvoidanceWeight : public Weight {

	Q_OBJECT;

public:
	enum AvoidanceDirection {AvoidanceDirectionLeft, AvoidanceDirectionRight};
	enum AvoidanceMode {NORMAL, DROP_AVOIDANCE, BUMPER_AVOIDANCE, COLLISION_ENCOUNTER};

private:
	AvoidanceMode mode;
	AvoidanceDirection avoidDirection;
	double scollisionPrevious;
	Vector2D positionPrevious;
	double rotationPrevious;
	Trafo2D tr1;
	Trafo2D tr2;
	Vector2D cp1;
	Vector2D cp2;

	Vector2D collisionPosition;

public:
	CollisionAvoidanceWeight(Core *core) : Weight("Collision Avoidance", core) {
		mode = NORMAL;
		scollisionPrevious = 0;
		avoidDirection = AvoidanceDirectionLeft;
		positionPrevious = core->tracker->getTranslation();
		rotationPrevious = core->tracker->getRotation();
	};

	virtual ~CollisionAvoidanceWeight() {};

	virtual void process(Vector2D &v) {

		// Check cliff sensors and bumpers and set mode
		bool cliffFrontLeft = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_CLIFF_FRONT_LEFT);
		bool cliffFrontRight = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_CLIFF_FRONT_RIGHT);
		bool cliffLeft = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_CLIFF_LEFT);
		bool cliffRight = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_CLIFF_RIGHT);
		bool bumperLeft = (core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_BUMP_LEFT) == COIL::BUMPWHEELDROP_BUMP_LEFT;
		bool bumperRight = (core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_BUMP_RIGHT) == COIL::BUMPWHEELDROP_BUMP_RIGHT;

		if ( mode != DROP_AVOIDANCE && (cliffFrontLeft || cliffLeft || cliffRight || cliffFrontRight) ){
			mode = DROP_AVOIDANCE;
			collisionPosition = core->tracker->getTranslation();
			Debug::print("[CollisionAvoidanceWeight] mode: DROP_AVOIDANCE");
		} else if ( mode != BUMPER_AVOIDANCE && (bumperLeft || bumperRight) ){
			mode = BUMPER_AVOIDANCE;
			collisionPosition = core->tracker->getTranslation();
			Debug::print("[CollisionAvoidanceWeight] mode: OBSTACLE_AVOIDANCE");
		}

		double av = (v.x() + v.y()) / 2.0; // Set our avoidance speed based on the input speed average. So if we are still anyways, we don't have to avoid anything. If we are rotating around our axis, we don't do anything.
		double af = 1.0; // Avoidance factor, determines the threshold for the urgency to avoid an obstacle. 1.0 means you will start to avoid the obstacle as soon as you detect it. 2.0 will avoid the obstacle before you detect it (virtually). 0.5 will only avoid the obstacle once you are half-way to hitting it since detection.
		double reverseDistance = 0.5;

		double scollision = (double) core->navigation->getMaxOpenAreaDistanceFromMap(Navigation::CollisionSourceFadingCollisionMap, Navigation::CollisionDirectionAhead);
		//double scollision = (double) core->coil->getIRSensorDistanceFromAnalogSignal(core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_ANALOG_SIGNAL));
		double smax = core->doubleSetting("Robot_SharpIRSensorCutoffValue");

		// Check status
		if (mode == DROP_AVOIDANCE || mode == BUMPER_AVOIDANCE){
			v = Vector2D(-Abs(av),-Abs(av));
			if( dist(collisionPosition, core->tracker->getTranslation()) > smax*reverseDistance) mode = NORMAL;

		} else {

			// NORMAL mode

			if(v.x() == -v.y()) {
				return; // test: allow rotation around axis
			}

			if(scollision < smax*af) {


//				if(mode == NORMAL) {
//					tr1 = core->movementTracker->transformation();
//					cp1 = (core->movementTracker->transformation() * Trafo2D::trans(0, scollision)).trans();
//					mode = COLLISION_ENCOUNTER;
//					Debug::print("[CollisionAvoidanceWeight] mode: COLLISION_ENCOUNTER");
//				}
//
//				tr2 = core->movementTracker->transformation();
//				cp2 = (core->movementTracker->transformation() * Trafo2D::trans(0, scollision)).trans();
//
//				double alpha = Deg(tr2.angle() - tr1.angle());
//
//				Vector2D cp2tr2 = tr2.trans() - cp2;
//				Vector2D cp1cp2 = cp1 - cp2;
//
//				double beta = Deg(acosf(dot(normalize(cp2tr2), normalize(cp1cp2))));
//
//				if(mode != NORMAL && dist(tr1.trans(), tr2.trans()) > 200) {
//
//					if(beta > 90 - alpha) {
//						//TODO: not yet working
//						//avoidDirection = (avoidDirection == AvoidanceDirectionLeft ? AvoidanceDirectionRight : AvoidanceDirectionLeft);
//						//Debug::print("[CollisionAvoidanceWeight]: change avoidance dir to %1", avoidDirection == AvoidanceDirectionLeft ? "left" : "right");
//					}
//					mode = NORMAL;
//					Debug::print("[CollisionAvoidanceWeight] mode: NORMAL");
//				}

				if(avoidDirection == AvoidanceDirectionLeft) {
					v = Vector2D(av * (scollision/(smax*af)), av); // left
				} else {
					v = Vector2D(av, av * (scollision/(smax*af))); // right
				}
			}

		}



	};
};

#endif /* COLLISIONAVOIDANCEWEIGHT_H_ */
