/*
 *  MoveTask.cpp
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

#include "MoveTask.h"

#include "../Controller/Controller.h"
#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"

MoveTask::MoveTask(QString name, Core *core, int interval, TaskPriority priority) :
	Task(name, core, interval, priority) {

	angleToTurn = 0.0;
	distanceToMove = 0.0;
	destination = Trafo2D::trans(0,0);

}

MoveTask::~MoveTask() {

}

int MoveTask::x() {
	return destination.trans().x();
}

int MoveTask::y() {
	return destination.trans().y();
}

// To move forward: direction = 1
// To move backward: direction = -1
void MoveTask::move(int distance, int direction) {

	//TODO: why is this so inaccurate? What is the timing problem?

	// Init
	distanceToMove = core->tracker->getTotalDistance() + distance;

	// Move, wait for distance, stop
	core->controller->setWheelSpeed(core->controller->targetSpeed*direction, core->controller->targetSpeed*direction);
	waitForDistance();
	core->controller->setWheelSpeed(0, 0);

}

void MoveTask::waitForDistance() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ( core->tracker->getTotalDistance() < distanceToMove) {
		SleeperThread::msleep(core->intSetting("Task_MoveTask_WaitForEventIntervalDelay"));
	}
}

void MoveTask::turn(int angle) {


	// Init
	angleToTurn = core->tracker->getTotalAngle() + std::abs((double)angle);
	distanceToMove = 0;

	// Move, wait for distance, stop
	if (angleToTurn > 0) {
		core->controller->setWheelSpeed(-core->controller->targetSpeed,+core->controller->targetSpeed);
	} else {
		core->controller->setWheelSpeed(+core->controller->targetSpeed,-core->controller->targetSpeed);
	}
	waitForAngle();
	core->controller->setWheelSpeed(0,0);
}

void MoveTask::waitForAngle() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ( core->tracker->getTotalAngle() < angleToTurn) {
		SleeperThread::msleep(core->intSetting("Task_MoveTask_WaitForEventIntervalDelay"));
	}

}

