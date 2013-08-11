/*
 *  Controller.h
 *
 *  The abstract Controller class is the base of fully functional Controllers.
 *  It provides the virtual methods run() and emergencyStop() which must be
 *  implemented by the child classes. The process routine of the Core Controller
 *  is the run() method and must only exit if the variable stopRequested is true.
 *  Currently, there are three different Controller implementations: Block Drive
 *  Controller, Fluid Drive Controller, and Emss Controller. Only the later is
 *  meant for real use, the others are for testing purposes only. In addition
 *  to sending movement commands to COIL, the Controller is also responsible for
 *  passing along sensor data to other components such as the Movement Tracker. This
 *  is realized in the form of Qt signals.
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

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <QQueue>
#include <QReadWriteLock>

#include "../CoreThread.h"
#include "../Library/Math.h"

class ThreadMonitorWatchdogAction;

class Controller : public CoreThread
{
	Q_OBJECT

protected:
	bool stopRequested;
	ThreadMonitorWatchdogAction *watchdogAction;
	QReadWriteLock lock;

public:
	int interval;
	int targetSpeed; // The target speed used by the controller and other components (such as Weights)
	int *sensorData; // Pointer to sensor data which is 36*sizeof(int)

public:
	Controller(QString name, Core *core, int interval);
	virtual ~Controller();
	virtual void process() = 0;
	virtual void emergencyStop();
	void run();
	void stop();
	void start(QThread::Priority priority);

private:
	virtual void _emergencyStop();

signals:
/* Signals for movement notification for a MovementTracker object */
	void signalMovedDistance(double distance);
	void signalChangedAngle(double angle);
	void signalCollision();
	void signalObjectDetected(double distance, double angle, double opacity, int size);
	void signalSensorDataUpdated();
	void signalChangedWheelSpeed(int, int);

public slots:
	virtual void setWheelSpeed(int Lwheel, int Rwheel) = 0;
	virtual void setTargetSpeed(int speed);
};

#endif /* CONTROLLER_H_ */
