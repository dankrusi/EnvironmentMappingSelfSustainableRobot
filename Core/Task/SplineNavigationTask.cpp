/*
 *  SplineNavigationTask.cpp
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

#include "SplineNavigationTask.h"

#include "../Controller/EmssController.h"
#include "../Library/Math.h"
#include "../Library/Debug.h"
#include "../Navigation/SplineNavigation.h"
#include "../Tracker/MovementTracker.h"

SplineNavigationTask::SplineNavigationTask(Core *core, int speed, TaskPriority priority) :
	Task("Spline Navigation", core, core->intSetting("Task_SplineNavigationTask_Interval"), priority) {
	this->speed = speed;
	tick = 0;
}

SplineNavigationTask::~SplineNavigationTask() {

}


void SplineNavigationTask::process() {

	// Extract navigation
	SplineNavigation *navigation = NULL;
	if(core->navigation->name != "Spline") {
		Debug::warning("[SplineNavigationTask] Navigation module is not of type Spline!");
		status = Task::Finished;
		return;
	} else {
		navigation = (SplineNavigation*)core->navigation;
	}

	// Align the robot and then crawl the spline...
	//if(tick == 0 && core->boolSetting("Task_SplineNavigationTask_AlignRobotWithSpline") == true) ((EmssController*) core->controller)->turn((int)(- core->movementTracker->rotation() + navigation->getAngleForSplineAlignment()), core->intSetting("Task_SplineNavigationTask_AlignRobotWithSplineSpeed_mmps"));
	if(tick == 0 && core->boolSetting("Task_SplineNavigationTask_AlignRobotWithSpline") == true) Debug::warning("[SplineNavigationTask] Task_SplineNavigationTask_AlignRobotWithSpline is deprecated!");

	// Send wheel speeds for current tick to controller
	Vector2D wheelSpeed = core->navigation->getWheelSpeed(tick, interval);
	core->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

	// Finished?
	if(wheelSpeed == Vector2D(0.0,0.0)) {
		// Reset?
		if(core->boolSetting("Navigation_SplineNavigation_ResetNavPointsWhenDone") == true) 	navigation->clearNavPoints();
		else 																				navigation->setTickOffset(tick);

		status = Task::Finished;
	}

	tick++;
}

QString SplineNavigationTask::description() {
	return QString("Speed:\t%2").arg(speed);
}
