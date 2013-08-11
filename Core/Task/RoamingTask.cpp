/*
 *  RoamingTask.cpp
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

#include "RoamingTask.h"

#include "../Navigation/Navigation.h"
#include "../Navigation/SystemOfWeightsNavigation.h"
#include "../Navigation/Weight/RoamingWeight.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"
#include "../Navigation/Weight/OrientationWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/JoystickWeight.h"

#include "../Controller/EmssController.h"

#include "../Library/Debug.h"

RoamingTask::RoamingTask(Core *core, int speed, TaskPriority priority) :
	Task("RoamingTask", core, core->intSetting("Task_DefaultInterval"), priority) {
	this->running = false;
}

RoamingTask::~RoamingTask() {

}

void RoamingTask::process() {

	if (running == false){
		if(  core->navigation->name == "System Of Weights"){
			navigation = (SystemOfWeightsNavigation*) core->navigation;
			navigation->removeAllWeights();
			navigation->addWeight(new FullSpeedWeight(core));
			navigation->addWeight(new RoamingWeight(core));
			navigation->addWeight(new JoystickWeight(core));
			navigation->addWeight(new CollisionAvoidanceWeight(core));
			navigation->addWeight(new AccelerationFilterWeight(core));
			navigation->addWeight(new ControllerSpeedWeight(core));
			running = true;
		}
		else {
			Debug::warning("This navigation module is not supported");
			this->status = Finished;
		}
	}

	Vector2D wheelSpeed = navigation->getWheelSpeed(0,0);
	core->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

}

void RoamingTask::postProcess() {
	core->controller->setWheelSpeed(0, 0);
}

QString RoamingTask::description() {
	return QString("Task:\tRoamingTask");
}
