/*
 * ThreadMonitorWatchdogAction.h
 *
 *  Created on: Mar 20, 2009
 *      Author: dankrusi
 */

#ifndef THREADMONITORWATCHDOGACTION_H_
#define THREADMONITORWATCHDOGACTION_H_

#include "WatchdogAction.h"

#include "../Library/Debug.h"

#include <QTime>

class ThreadMonitorWatchdogAction : public WatchdogAction
{
	Q_OBJECT

#define QTIME_NULL QTime(0,0,0,0)

private:
	QTime lastPingFromThread;
	QString component;
	int expectedPingInterval;
	bool isActive;
	double flexibility;

public:

	ThreadMonitorWatchdogAction(QString component, int expectedPingInterval, double flexibility, Core *core) : WatchdogAction("Thread Monitor", core) {
		this->component = component;
		this->expectedPingInterval = expectedPingInterval;
		lastPingFromThread = QTIME_NULL;
		isActive = false;
		this->flexibility = flexibility;
	}

	virtual ~ThreadMonitorWatchdogAction() {}

	virtual void process() {

		if(isActive && lastPingFromThread.elapsed() > expectedPingInterval*flexibility) {
			Debug::warning("[ThreadMonitorWatchdogAction] component %1 is locked", component);
			core->abort(false);
		}
	}

	virtual QString description() {
		return QString("Name:\t%1").arg(name);
	}

	void ping() {
		lastPingFromThread.restart();
	}

	void ping(int expectedPingInterval) {
		this->expectedPingInterval = expectedPingInterval;
		ping();
	}

	void setActive(bool active) {
		isActive = active;
		ping();
	}

	bool active() {
		return isActive;
	}

};

#endif /* THREADMONITORWATCHDOGACTION_H_ */
