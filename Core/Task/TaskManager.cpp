/*
 *  TaskManager.cpp
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

#include "TaskManager.h"

#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"

#include "../Task/Task.h"

#include "../Controller/EmssController.h"

#include "../Watchdog/Watchdog.h"
#include "../Watchdog/ThreadMonitorWatchdogAction.h"

#include "JoystickNavigationTask.h"

TaskManager::TaskManager(Core *core) : CoreThread("TaskManager", core){
	this->core = core;

	currentTask = NULL;
	idleTask = NULL;
	stopRequested = false;
	tasks = new TaskList();
	watchdogAction = NULL;

	// Watchdog this?
	if(core->watchdog && core->boolSetting("TaskManager_Watchdog_Enabled")) {
		watchdogAction = new ThreadMonitorWatchdogAction(QString("Task Manager"), core->intSetting("TaskManager_Interval"), core->doubleSetting("TaskManager_Watchdog_Flexibility"), core);
		core->watchdog->addAction(watchdogAction);
	}
}

TaskManager::~TaskManager() {
	if(watchdogAction && core->watchdog) core->watchdog->removeAction(watchdogAction);
	if(watchdogAction) delete watchdogAction;
	if(idleTask) delete idleTask;
	delete tasks;
}

void TaskManager::run() {

	//TODO: make sure the idle task is cleanly pre/post processed etc...

	// Enter processing loop...
	stopRequested = false;
	while (stopRequested == false) {

		if (currentTask != NULL && currentTask->status == Task::Running) {

			// Process the task
			if(!currentTask->interruptRequested()) {

				currentTask->process();

			} else {

				currentTask->status = Task::Interrupted;

			}

			// Did we finish?
			if (currentTask->status == Task::Finished) {

				currentTask->postProcess();
				emit taskListChanged();
				Debug::print("[TaskManager] %1 Task finished", currentTask->getName());

			} else if (currentTask->status == Task::Interrupted) {

				currentTask->postProcess();
				emit taskListChanged();
				Debug::warning("[TaskManager] %1 Task interrupted", currentTask->getName());

			} else {

				// Sleep our task interval...
				if(watchdogAction) watchdogAction->ping(currentTask->getInterval());
				this->msleep(currentTask->getInterval());

			}

		} else {

			// Try to get a new task
			currentTask = tasks->getNextTask();

			if (currentTask == NULL) {
				// No task available... either wait the interval or process the idle task
				if(idleTask) {
					// Process the idle task. If not already running, pre process...
					if(idleTask->status != Task::Running) {
						idleTask->preProcess();
						idleTask->status = Task::Running;
					}
					idleTask->process();
					if(watchdogAction) watchdogAction->ping(idleTask->getInterval());
					this->msleep(idleTask->getInterval());
				} else {
					int interval = core->intSetting("TaskManager_Interval");
					if(watchdogAction) watchdogAction->ping(interval);
					this->msleep(interval);
				}
			} else if (currentTask->status == Task::Waiting) {
				// Found a task which is waiting to be run!
				if(idleTask) {
					idleTask->postProcess();
					idleTask->status = Task::Interrupted;
				}
				currentTask->preProcess();
				currentTask->status = Task::Running;
				emit taskListChanged();
				Debug::print("[TaskManager] %1 Task started", currentTask->getName());
			}
		}

	}
}

void TaskManager::stop() {

	// Request a stop and wait for the controller to exit its process loop
	stopRequested = true;
	while(this->isRunning() == true) SleeperThread::msleep(50);
	if(watchdogAction) watchdogAction->setActive(false);

	Debug::print("[TaskManager] stopped");

}

void TaskManager::addTask(Task *task) {
	Debug::print("[TaskManager] %1 Task added", task->getName());
	tasks->add(task);
	emit taskListChanged();
}

void TaskManager::start(QThread::Priority priority) {

	Debug::print("[TaskManager] started");

	if(watchdogAction) watchdogAction->setActive(true);
	QThread::start(priority);
}

Task* TaskManager::getCurrentTask() {
	return currentTask;
}

Task* TaskManager::getIdleTask() {
	return idleTask;
}

void TaskManager::setIdleTask(Task *idleTask) {
	this->idleTask = idleTask;
}
