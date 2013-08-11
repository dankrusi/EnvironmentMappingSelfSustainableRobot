/*
 *  Watchdog.cpp
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

#include "Watchdog.h"

#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"

#include "../Watchdog/WatchdogAction.h"

Watchdog::Watchdog(Core *core, int interval) : CoreThread("Watchdog", core){

	this->interval = interval;

	stopRequested = false;
	actions = new QList<WatchdogAction*>();
}

Watchdog::~Watchdog() {
	delete actions;
}

void Watchdog::run() {

	// Enter processing loop
	stopRequested = false;
	while (stopRequested == false) {

		// Is it time again to process our actions?
		if(lastActionProcess.elapsed() > core->intSetting("Watchdog_ActionInterval")) {

			lastActionProcess.restart();

			// Lockup and process all actions
			_mutex.lock(); {
				for(int i = 0; i < actions->count(); i++) {
					actions->at(i)->process();
				}
			} _mutex.unlock();
		}

		// Sleep our interval...
		this->msleep(interval);

	}
}

void Watchdog::stop() {

	// Request a stop and wait for the watchdog to exit its process loop
	stopRequested = true;
	while(this->isRunning() == true) SleeperThread::msleep(core->intSetting("Watchdog_StopRequestInterval"));

	Debug::print("[Watchdog] stopped");

}

void Watchdog::start(QThread::Priority priority) {

	Debug::print("[Watchdog] started");

	QThread::start(priority);
}


void Watchdog::addAction(WatchdogAction *action) {
	// Lockup and add to list of actions
	_mutex.lock(); {
		actions->append(action);
	} _mutex.unlock();
	Debug::print("[Watchdog] added %1 Action", action->name);
}

void Watchdog::removeAction(WatchdogAction *action) {
	// Lockup, remove from list, and delete
	if(!action) return;
	QString name = action->name;
	_mutex.lock(); {
		actions->removeAll(action);
	} _mutex.unlock();
	Debug::print("[Watchdog] remove %1 Action", name);
}
