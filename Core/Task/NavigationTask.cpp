/*
 *  NavigationTask.cpp
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

#include "NavigationTask.h"

#include "../Controller/EmssController.h"
#include "../Library/Math.h"
#include "../Library/Debug.h"
#include "../Navigation/Navigation.h"
#include "../Navigation/SystemOfWeightsNavigation.h"
#include "../Tracker/MovementTracker.h"

#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/OrientationWeight.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/JoystickWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"



//TODO: Settings, finish check, et cetera... This class is not finished at all!

NavigationTask::NavigationTask(Core *core, int speed, TaskPriority priority) :
	Task("Navigation", core, core->intSetting("Task_NavigationTask_Interval"), priority) {
	this->speed = speed;
	tick = 0;
}

NavigationTask::~NavigationTask() {

}

void NavigationTask::preProcess() {
	if(  core->navigation->name == "System Of Weights"){
		((SystemOfWeightsNavigation *)core->navigation)->removeAllWeights();
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new FullSpeedWeight(core));
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new OrientationWeight(core));
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new JoystickWeight(core));
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new CollisionAvoidanceWeight(core));
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new AccelerationFilterWeight(core));
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new ControllerSpeedWeight(core));
	}
	else if (core->navigation->name == "Spline"){
			// using SplineNavigation
		}
}

void NavigationTask::process() {

	// Send wheel speeds for current tick to controller
	Vector2D wheelSpeed = core->navigation->getWheelSpeed(tick, interval);
	core->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

	// Finished?
	if(core->navigation->isAtLastNavPoint()) {
		// Reset?
		if(core->boolSetting("Task_NavigationTask_ResetNavPointsWhenDone") == true) core->navigation->clearNavPoints();
		status = Task::Finished;
	}

	tick++;
}

void NavigationTask::postProcess() {
	core->controller->setWheelSpeed(0, 0);
}

QString NavigationTask::description() {
	return QString("Speed:\t%2").arg(speed);
}
