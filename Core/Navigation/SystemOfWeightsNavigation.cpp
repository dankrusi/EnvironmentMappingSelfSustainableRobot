/*
 *  SystemOfWeightsNavigation.cpp
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

#include "SystemOfWeightsNavigation.h"

#include "Weight/Weight.h"
#include "Weight/FullSpeedWeight.h"
#include "Weight/ControllerSpeedWeight.h"
#include "Weight/OrientationWeight.h"
#include "Weight/CollisionAvoidanceWeight.h"
#include "Weight/AccelerationFilterWeight.h"

SystemOfWeightsNavigation::SystemOfWeightsNavigation(Core *core, bool createDefaultWeights) : Navigation("System Of Weights", core) {
	// Init
	weights = new QList<Weight*>();

	// Default weights...
	if(createDefaultWeights) {
		weights->append(new FullSpeedWeight(core));
		weights->append(new OrientationWeight(core));
		weights->append(new AccelerationFilterWeight(core));
		weights->append(new CollisionAvoidanceWeight(core));
		weights->append(new ControllerSpeedWeight(core));
		emit weightsChanged();
	}

	stopAtLastNavPoint = core->boolSetting("Navigation_SystemOfWeightsNavigation_StopAtLastNavPoint");
}

SystemOfWeightsNavigation::~SystemOfWeightsNavigation() {
	delete weights;
}

Vector2D SystemOfWeightsNavigation::getWheelSpeed(int tick, int interval) {

	// Are we at a destination?
	Vector2D destination = getCurrentDestination();
	Vector2D position = core->tracker->getTranslation();
	double tolerance = 200;
	if(    position.x() >= destination.x()-tolerance
		&& position.x() <= destination.x()+tolerance
		&& position.y() >= destination.y()-tolerance
		&& position.y() <= destination.y()+tolerance ) {
		// Continue to next destination
		currentNavPoint++;
		if(isAtLastNavPoint() && stopAtLastNavPoint) return Vector2D(0.0,0.0); // Stop at the last point...
	}

	// Go through all the weights, each modifying our velocity vector
	Vector2D v(0.0,0.0);
	lock.lockForRead(); {
		for(int i = 0; i < weights->size(); i++) {
			// Reference to our v, process() will modify it...
			weights->at(i)->process(v);
		}
	} lock.unlock();

	// Return a copy of the final velocity vector
	return v;
}

const QList<Weight*> *SystemOfWeightsNavigation::getWeights() {
	return weights;
}

void SystemOfWeightsNavigation::removeAllWeights() {
	lock.lockForWrite(); {
		weights->clear();
	} lock.unlock();
	emit weightsChanged();
}

void SystemOfWeightsNavigation::addWeight(Weight* weight){
	lock.lockForWrite(); {
		weights->append(weight);
	} lock.unlock();
	emit weightsChanged();
}

bool SystemOfWeightsNavigation::removeWeight(QString name){

	bool retVal = false;
	lock.lockForWrite(); {
		for(int i = 0; i < weights->count(); i++) {
				if (weights->at(i)->name == name){
					weights->removeAt(i);
					retVal = true;
				}
		}
	} lock.unlock();
	emit weightsChanged();
	return retVal;

}

bool SystemOfWeightsNavigation::removeWeight(Weight* weight) {
	bool retVal;
	lock.lockForWrite(); {
		retVal = weights->removeOne(weight);
	} lock.unlock();
	emit weightsChanged();
	return retVal;
}

void SystemOfWeightsNavigation::moveWeightDown(Weight* weight) {
	lock.lockForWrite(); {
		for(int i = 0; i < weights->count(); i++) {
			if (weights->at(i) == weight){
				weights->swap(i,i+1);
				break;
			}
		}
	} lock.unlock();
	emit weightsChanged();
}

void SystemOfWeightsNavigation::moveWeightUp(Weight* weight) {
	lock.lockForWrite(); {
		for(int i = 0; i < weights->count(); i++) {
			if (weights->at(i) == weight){
				weights->swap(i,i-1);
				break;
			}
		}
	} lock.unlock();
	emit weightsChanged();
}

