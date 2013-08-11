/*
 *  JoystickNavigationTask.cpp
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
#include "JoystickNavigationTask.h"

#include "../Library/Math.h"
#include "../Library/Debug.h"
#include "../Navigation/Navigation.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/JoystickWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"

JoystickNavigationTask::JoystickNavigationTask(Core *core, TaskPriority priority) :
	Task("JoystickNavigationTask", core, core->intSetting("Task_JoystickNavigationTask_Interval"), priority ){
}

JoystickNavigationTask::~JoystickNavigationTask(){

}

void JoystickNavigationTask::preProcess() {
	if(  core->navigation->name == "System Of Weights"){
		navigation = (SystemOfWeightsNavigation*) core->navigation;
		navigation->removeAllWeights();
		navigation->addWeight(new JoystickWeight(core));
		navigation->addWeight(new CollisionAvoidanceWeight(core));
		navigation->addWeight(new AccelerationFilterWeight(core));
		navigation->addWeight(new ControllerSpeedWeight(core));
	} else {
		Debug::warning("[JoystickNavigationTask] This navigation module is not supported");
		this->status = Interrupted;
	}
}

void JoystickNavigationTask::process() {

	if(core->navigation->name != "System Of Weights") return;

	Vector2D wheelSpeed = navigation->getWheelSpeed(0,0);
	core->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

}

void JoystickNavigationTask::postProcess() {
	core->controller->setWheelSpeed(0, 0);
}

QString JoystickNavigationTask::description() {
	return "";
}
