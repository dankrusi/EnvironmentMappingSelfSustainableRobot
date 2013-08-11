/*
 * CoreFactory.cpp
 *
 *  Created on: Mar 29, 2009
 *      Author: dankrusi
 */

#include "CoreFactory.h"

#include "Core.h"

#include "COIL/COIL.h"
#include "COIL/EmulatedCOIL.h"

#include "Library/Debug.h"
#include "Library/Util.h"

#include "Map/GridMap.h"
#include "Map/HeatMap.h"
#include "Map/TerrainMap.h"
#include "Map/ObjectMap.h"
#include "Map/PhysicalMap.h"
#include "Map/StructureMap.h"
#include "Map/FadingCollisionMap.h"

#include "Object/RobotObject.h"
#include "Object/NavPathObject.h"
#include "Object/NavigationObject.h"
#include "Object/MarkerObject.h"

#include "Tracker/AveragedTracker.h"
#include "Tracker/SingleTracker.h"
#include "Tracker/RawMovementTracker.h"
#include "Tracker/ExpectedMovementTracker.h"

#include "Controller/EmssController.h"

#include "Navigation/SplineNavigation.h"
#include "Navigation/SystemOfWeightsNavigation.h"

#include "Task/Task.h"
#include "Task/TaskManager.h"

#include "RemoteInterface/RemoteInterface.h"

#include "Watchdog/Watchdog.h"
#include "Watchdog/BeepWatchdogAction.h"
#include "Watchdog/NetworkMonitorWatchdogAction.h"


COIL* CoreFactory::createCOIL(Core *core, QString name) {
	if (name == "Emulated"){
		return new EmulatedCOIL(core);
	} else if (name == "Serial Port") {
		return new COIL(core->connectionSerialPort, core);
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid COIL!", name);
		return NULL;
	}
}

Tracker* CoreFactory::createTracker(Core *core, QString name) {

	// Create the correct implementation
	Tracker *tracker;
	if (name == "Averaged"){
			tracker = new AveragedTracker(core);
	} else if (name == "Single"){
		tracker = new SingleTracker(core);
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid Tracker!", name);
		return NULL;
	}

	// Create and connect objects related to this tracker
	RobotObject *robot = new RobotObject(core->objectMap, tracker->getTransformation().trans().x(), tracker->getTransformation().trans().y(), core->longSetting("Robot_Diameter_mm"), core->longSetting("Robot_Diameter_mm"), Qt::white, 1.0);
	QObject::connect(tracker, SIGNAL(moved(long, long, double)), robot, SLOT(move(long,long,double)));
	NavPathObject *navPath = new NavPathObject(core->objectMap, Qt::white);
	QObject::connect(tracker, SIGNAL(moved(long, long, double)), navPath, SLOT(addNavPoint(long,long)));

	return tracker;
}

MovementTracker* CoreFactory::createMovementTracker(Core *core, QString name) {

	// Create the correct implementation
	MovementTracker *movementTracker;
	QColor color;
	if(name == "Raw") {
		movementTracker = new RawMovementTracker(core, core->longSetting("Robot_StartingPositionX_mm"), core->longSetting("Robot_StartingPositionY_mm"), 0);
		color = Qt::blue;
	} else if(name == "Expected") {
		movementTracker = new ExpectedMovementTracker(core, core->longSetting("Robot_StartingPositionX_mm"), core->longSetting("Robot_StartingPositionY_mm"), 0);
		color = Qt::red;
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid Movement Tracker!", name);
		return NULL;
	}

	// Create and connect objects related to this movement tracker
	RobotObject *robot = new RobotObject(core->objectMap, movementTracker->getTransformation().trans().x(), movementTracker->getTransformation().trans().y(), core->longSetting("Robot_Diameter_mm"), core->longSetting("Robot_Diameter_mm"), color, 0.5);
	QObject::connect(movementTracker, SIGNAL(moved(long, long, double)), robot, SLOT(move(long,long,double)));
	NavPathObject *navPath = new NavPathObject(core->objectMap, color);
	QObject::connect(movementTracker, SIGNAL(moved(long, long, double)), navPath, SLOT(addNavPoint(long,long)));

	return movementTracker;
}


Navigation* CoreFactory::createNavigation(Core *core, QString name) {

	// Create the navigation module specified by name
	Navigation *navigation;
	if(name == "Spline") {
		navigation = new SplineNavigation(core);
	} else if (name == "System of Weights") {
		navigation = new SystemOfWeightsNavigation(core, true);
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid Navigation!", name);
		return NULL;
	}

	// Attach a new navigation object to draw this navigation module...
	new NavigationObject(core->objectMap, navigation);

	return navigation;
}

TaskManager* CoreFactory::createTaskManager(Core *core) {
	return new TaskManager(core);
}

Controller* CoreFactory::createController(Core *core, QString name) {
	if (name == "emss") {
		return new EmssController(core, core->intSetting("Controller_EmssController_Speed"), core->intSetting("Controller_EmssController_Interval"));
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid Controller!", name);
		return NULL;
	}
}

RemoteInterface* CoreFactory::createRemoteInterface(Core *core) {
	return new RemoteInterface(core, core->stringSetting("RemoteInterface_IP"), core->intSetting("RemoteInterface_Port"));
}

Watchdog* CoreFactory::createWatchdog(Core *core) {
	// Create a new watchdog and add default actions
	Watchdog *watchdog = new Watchdog(core, core->intSetting("Watchdog_Interval"));
	//TODO: delete these actions...
	if(core->boolSetting("Watchdog_AddDefaultBeepAction")) watchdog->addAction(new BeepWatchdogAction(core));
	if(core->boolSetting("Watchdog_AddDefaultNetworkMonitorAction")) watchdog->addAction(new NetworkMonitorWatchdogAction(core));

	return watchdog;
}

