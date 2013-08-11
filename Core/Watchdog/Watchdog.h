/*
 *  Watchdog.h
 *
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_


#include <QObject>
#include <QThread>
#include <QList>
#include <QTime>
#include <QMutex>

#include "../CoreThread.h"

class WatchdogAction;

class Watchdog : public CoreThread
{
	Q_OBJECT

protected:
	bool stopRequested;
	QList<WatchdogAction*> *actions;
	QMutex _mutex;
	QTime lastActionProcess;

public:
	int interval;

public:
	Watchdog(Core *core, int interval);
	virtual ~Watchdog();
	virtual void run();
	void stop();
	void start(QThread::Priority priority);
	void addAction(WatchdogAction *action);
	void removeAction(WatchdogAction *action);

};

#endif /* WATCHDOG_H_ */
