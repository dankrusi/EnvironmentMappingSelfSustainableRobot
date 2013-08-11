/*
 *  FingerprintNavigationTask.cpp
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

#include "FingerprintNavigationTask.h"

#include "../Library/SleeperThread.h"
#include "../RemoteInterface/RemoteInterface.h"
#include "../RemoteInterface/RemoteInterfaceMessage.h"

#include "../Navigation/Weight/WallFollowerWeight.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"
#include "../Navigation/Weight/OrientationWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/JoystickWeight.h"

#include "../Map/StructureMap.h"
#include "../Tracker/Tracker.h"

#include "../Library/Math.h"
#include "../Library/Util.h"

#include <cstdlib>

FingerprintNavigationTask::FingerprintNavigationTask(Core *core, long fingerprintInterval, long fingerprintWaitTime, TaskPriority priority) :
	Task("FingerprintNavigation", core, core->intSetting("Task_FingerprintNavigationTask_Interval"), priority ){
	this->fingerprintInterval = fingerprintInterval;
	this->fingerprintWaitTime = fingerprintWaitTime;
	lastFingerprint = QTime(0,0,0,0); // never!
}

FingerprintNavigationTask::~FingerprintNavigationTask(){

}

void FingerprintNavigationTask::preProcess() {
	if(  core->navigation->name == "System Of Weights"){
		navigation = (SystemOfWeightsNavigation*) core->navigation;
		navigation->removeAllWeights();
		navigation->addWeight(new FullSpeedWeight(core));
		navigation->addWeight(new WallFollowerWeight(core));
		navigation->addWeight(new JoystickWeight(core));
		navigation->addWeight(new CollisionAvoidanceWeight(core));
		navigation->addWeight(new AccelerationFilterWeight(core));
		navigation->addWeight(new ControllerSpeedWeight(core));
	}
	else {
		Debug::warning("This navigation module is not supported");
		this->status = Interrupted;
	}
}

void FingerprintNavigationTask::process(){

	// Follow the wall
	Vector2D wheelSpeed = navigation->getWheelSpeed(0,0);
	core->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

	// Is it time for a fingerprint?
	if(QTime::currentTime() > lastFingerprint.addMSecs(fingerprintInterval + fingerprintWaitTime)) {

		// Create and send message with emulated data
		if(core->remoteInterface) {

			// Send a fingerprint message...
			RemoteInterfaceMessage *message = new RemoteInterfaceMessage("fingerprint");

			// Local position
			message->properties->insert("local-position-x", QString("%1").arg(core->tracker->getX()));
			message->properties->insert("local-position-y", QString("%1").arg(core->tracker->getY()));

			// Get GPS coordinates based on an offset added with the degrees based on x,y meters at that offset
			GPSPosition gps = Util::gpsPositionFromString(core->stringSetting("Map_GPSOffset"));
			gps.latitude += gps.latitudesPerMeters(core->tracker->getX() / 1000.0); // convert mm to m
			gps.longitude += gps.longitudesPerMeters(core->tracker->getY() / 1000.0); // convert mm to m
			message->properties->insert("global-position-latitude", QString("%1").arg(gps.latitude));
			message->properties->insert("global-position-longitude", QString("%1").arg(gps.longitude));

			// Add additional map information
			message->properties->insert("floor", core->stringSetting("Map_Floor"));
			message->properties->insert("map-description", core->stringSetting("Map_Description"));
			message->properties->insert("map-file", core->stringSetting("Map_PhysicalMap_Filename"));
			message->properties->insert("wait-time", QString("%1").arg(fingerprintWaitTime));

			// Send off the message as a broadcast to all connected
			core->remoteInterface->broadcastMessage(message);
		}
		else {
			Debug::warning("[FingerprintNavigationTask] no RemoteInterface available for sending fingerprint message!");
		}

		// Stop the robot and wait
		core->controller->setWheelSpeed(0, 0);
		lastFingerprint = QTime::currentTime();
		SleeperThread::msleep(fingerprintWaitTime);
	}

	// Is Task finished
	if (core->structureMap->isFinish() == true){
		this->status = Finished;
		Debug::print("[FingerprintTask] FingerprintTask Finished");
	}

}

void FingerprintNavigationTask::postProcess() {
	navigation->removeAllWeights();
	core->controller->setWheelSpeed(0, 0);
}

QString FingerprintNavigationTask::description() {
	return QString("FingerprintWaitTime:\t%1\nFingerprintInterval:%2\t").arg(fingerprintWaitTime).arg(fingerprintInterval);
}

