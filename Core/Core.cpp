/*
 *  Core.cpp
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

#include <assert.h>

#include "Core.h"
#include "CoreFactory.h"

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

#include "Tracker/Tracker.h"
#include "Tracker/AveragedTracker.h"
#include "Tracker/SingleTracker.h"
#include "Tracker/RawMovementTracker.h"
#include "Tracker/ExpectedMovementTracker.h"

#include "Controller/EmssController.h"

#include "Navigation/SplineNavigation.h"
#include "Navigation/SystemOfWeightsNavigation.h"

#include "Task/Task.h"
#include "Task/JoystickNavigationTask.h"

#include "Task/TaskManager.h"

#include "RemoteInterface/RemoteInterface.h"

#include "Watchdog/Watchdog.h"
#include "Watchdog/BeepWatchdogAction.h"
#include "Watchdog/NetworkMonitorWatchdogAction.h"

#include "Library/Joystick2D.h"
#include "Library/HeapLogger.h"

Core::Core() {

	// NULL everything for safety of deletion
	coil = NULL;

	gridMap = NULL;
	heatMap = NULL;
	terrainMap = NULL;
	objectMap = NULL;
	physicalMap = NULL;
	structureMap = NULL;
	fadingCollisionMap = NULL;

	navigationObject = NULL;
	markerObject = NULL;

	tracker = NULL;
	controller = NULL;

	navigation = NULL;

	taskManager = NULL;

	remoteInterface = NULL;

	watchdog = NULL;

	settings = NULL;

	joystick = new Joystick2D();;

	// Load settings and default values if needed
	settings = new QSettings(QString("%1emssCore.config").arg(Util::getResourcesFilePath()), QSettings::IniFormat);

	// Init
	connected = false;

	// Init maps and objects
	initMapsAndObjects();

	Debug::print("[Core] initialized");
}

Core::~Core() {

	// Free heap...
	if(coil) delete coil;

	if(gridMap) delete gridMap;
	if(heatMap) delete heatMap;
	if(terrainMap) delete terrainMap;
	if(objectMap) delete objectMap;
	if(physicalMap) delete physicalMap;
	if(structureMap) delete structureMap;
	if(fadingCollisionMap) delete fadingCollisionMap;

	if(navigationObject) delete navigationObject;
	if(markerObject) delete markerObject;

	if(tracker) delete tracker;
	if(controller) delete controller;

	if(navigation) delete navigation;

	if(taskManager) delete taskManager;

	if(remoteInterface) delete remoteInterface;

	if(watchdog) delete watchdog;

	if(settings) delete settings;

	if (joystick) delete joystick;
}

bool Core::connect(QString strController, QString strTracker, QString strNavigation, QString strSerialPort, bool safeMode, bool emulation) {

	// Save connection state
	connectionController = strController;
	connectionTracker = strTracker;
	connectionNavigation = strNavigation;
	connectionSerialPort = strSerialPort;
	connectionSafeMode = safeMode;
	connectionEmulation = emulation;

	// Init the watchdog
	if(watchdog) { delete watchdog; watchdog = NULL; }
	if(boolSetting("Watchdog_Enabled")) {
		watchdog = CoreFactory::createWatchdog(this);
		assert(watchdog);
		watchdog->start(QThread::NormalPriority); // We start/stop the watchdog here at connect/disconnect
	}

	// Init COIL
	if(coil) { delete coil; coil = NULL; }
	if (emulation){
		Debug::print("[Core] using EmulatedCOIL");
		coil = CoreFactory::createCOIL(this, "Emulated");
	} else {
		coil = CoreFactory::createCOIL(this, "Serial Port");
	}
	assert(coil);

	// Start OI
	if(coil->startOI() == -1 ) {
		Debug::error("[Core] could not start OI");
		delete coil;
		coil = NULL;
		return false;
	}
	Debug::print("[Core] started OI");

	// Enter the desired op mode on the controller...
	if(safeMode) {
		// Enter safe mode
		if(coil->enterSafeMode() == -1 ) {
			Debug::error("[Core] could not enter safe mode");
			delete coil;
			coil = NULL;
			return false;
		}
		Debug::print("[Core] entered safe mode");
	} else {
		// Enter full mode
		if(coil->enterFullMode() == -1 ) {
			Debug::error("[Core] could not enter full mode");
			delete coil;
			coil = NULL;
			return false;
		}
		Debug::print("[Core] entered full mode");
	}

	// Restet LED's
	coil->setLEDState(COIL::LED_ADVANCE | COIL::LED_PLAY, 0, 255);

	// Reset objectmap
	//TODO: delete the objects also!
	objectMap->objects->clear();

	// Init tracker module
	if(tracker) { delete tracker; tracker = NULL; }
	tracker = CoreFactory::createTracker(this, strTracker);
	assert(tracker);

	// Init movement trackers based on a semicolon seperated list from settings...
	if(stringSetting("Tracker_DefaultMovementTrackers") != "") {
		foreach(QString trackerToAdd, stringSetting("Tracker_DefaultMovementTrackers").split(" ")) {
			tracker->addMovementTracker(CoreFactory::createMovementTracker(this, trackerToAdd));
		}
	}

	// Do we need to set a selected Movement Tracker for the Single Tracker?
	if(tracker->name == "Single") {
		((SingleTracker*)tracker)->setSelectedMovementTracker(stringSetting("Tracker_SelectedMovementTrackers"));
	}

	// Init controller
	if(controller) { delete controller; controller = NULL; }
	controller = CoreFactory::createController(this, strController);
	assert(controller);

	// Create new navigation set
	if(navigation) { delete navigation; navigation = NULL; }
	navigation = CoreFactory::createNavigation(this, strNavigation);
	assert(navigation);

	// Init the task manager
	if(taskManager) { delete taskManager; taskManager = NULL; }
	taskManager = CoreFactory::createTaskManager(this);
	taskManager->setIdleTask(new JoystickNavigationTask(this));
	assert(taskManager);

	// Init the remote interface
	if(remoteInterface) { delete remoteInterface; remoteInterface = NULL; }
	if(boolSetting("RemoteInterface_Enabled")) {
		remoteInterface = CoreFactory::createRemoteInterface(this);
		assert(remoteInterface);
	}

	// Register misc. stuff with objects
	tracker->connectController(controller);
	structureMap->connectController(controller);
	fadingCollisionMap->connectController(controller);
	heatMap->connectController(controller);

	// Return success
	connected = true;
	Debug::print("[Core] connected");
	return true;
}

void Core::run() {

	// Kickoff all the threads
	if(controller) controller->start(QThread::NormalPriority);
	if(taskManager) taskManager->start(QThread::NormalPriority);
	if(remoteInterface) remoteInterface->start(QThread::NormalPriority);
	Debug::print("[Core] running...");
}

void Core::stop() {

	// Shutdown all the threads
	if(taskManager) taskManager->stop();
	if(controller) controller->stop();
	if(remoteInterface) remoteInterface->stop();
	Debug::print("[Core] stopped");
}

// This aborts the current connection. If killThreads is specified
// then the behaviour is undefined and can result in a crash. The
// idea of this method is to atleast send a emergencyStop signal
// to the hardware and then try to disconnect even if a thread has
// blocked due to a fault algorithm or task.
void Core::abort(bool killThreads) {

	Debug::warning("[Core] aborting...");

	// Ask controller to emergency stop
	if(controller) controller->emergencyStop();

	// Kill threads
	if(killThreads) {
		if(taskManager) taskManager->terminate();
		if(controller) controller->terminate();
		if(remoteInterface) remoteInterface->terminate();
		taskManager = NULL;
		controller = NULL;
		remoteInterface = NULL;
	}

	// Stop and disconnect
	stop();
	disconnect();

}

bool Core::disconnect() {

	// Shutdown coil
	if(coil) {
		coil->setLEDState(0, 255, 255);
		coil->stopOI();
		delete coil;
		coil = NULL;
	}

	// Stop the watchdog
	if(watchdog) watchdog->stop();

	// Delete all connection related components
//	if(tracker) { delete tracker; tracker = NULL; }
//	if(controller) { delete controller; controller = NULL; }
//	if(navigation) {
//		if(navigationObject) navigationObject->setNavigation(NULL);
//		//delete navigation;
//		//navigation = NULL;
//	}
//	if(taskManager) { delete taskManager; taskManager = NULL; }
//	if(remoteInterface) { delete remoteInterface; remoteInterface = NULL; }

	// Return success
	Debug::print("[Core] disconnected");
	connected = false;

	//Check if all heap objects are deleted
	//HeapLogger::printMemoryinUse();

	return true;
}

bool Core::isConnected() {
	return connected;
}

void Core::robotMoved(long x, long y, double rotation) {

//	// Update robot location
//	robotObject->x = (x);
//	robotObject->y = (y);
//	robotObject->rotation = -rotation; // Core/COIL has a anti-clockwise positive rotation
//
//	// Update nav path and heat map
//	navPathObject->addNavPoint(x,y);
//	emit focusOnPoint(x, y); // Signal to focus onto this point
}

long Core::mmToPixels(long mm) {
	return mm / scale;
}

long Core::pixelsTomm(long pixels) {
	return pixels * scale;
}

void Core::addTask(Task *task) {
	taskManager->addTask(task);
}

void Core::initMapsAndObjects(){

	// Grab latest settings
	this->scale = this->longSetting("Map_Scale");
	this->mapWidth = this->longSetting("Map_Width_mm");
	this->mapHeight = this->longSetting("Map_Height_mm");

	// Destroy all objects
	if(gridMap) delete gridMap;
	if(heatMap) delete heatMap;
	if(terrainMap) delete terrainMap;
	if(objectMap) delete objectMap;
	if(physicalMap) delete physicalMap;
	if(structureMap) delete structureMap;
	if(fadingCollisionMap) delete fadingCollisionMap;
	//if(navigationObject) delete navigationObject;
	//if(markerObject) delete markerObject;

	// Re-allocate all objects...
	gridMap = new GridMap(this, longSetting("Map_GridMap_GridSize_mm"));
	heatMap = new HeatMap(this, mapWidth, mapHeight);
	terrainMap = new TerrainMap(this, mapWidth, mapHeight);
	objectMap = new ObjectMap(this);
	physicalMap = new PhysicalMap(this, QString("%1%2").arg(Util::getResourcesFilePath()).arg(this->stringSetting("Map_PhysicalMap_Filename")), this->longSetting("Map_PhysicalMap_Scale"));
	structureMap = new StructureMap(this);
	fadingCollisionMap = new FadingCollisionMap(this);
}

void Core::reset(){
	if(connected == true) {
		stop();
		disconnect();
		initMapsAndObjects();
		connect(connectionController, connectionTracker, connectionNavigation, connectionSerialPort, connectionSafeMode, connectionEmulation);
		run();
	} else {
		initMapsAndObjects();
	}
	emit coreReset();
}

int Core::intSetting(QString key) {
	return getSetting(key).toInt(NULL);
}

long Core::longSetting(QString key) {
	return getSetting(key).toInt(NULL); //TODO: Is this correct?
}

double Core::doubleSetting(QString key) {
	return getSetting(key).toDouble(NULL);
}

QString Core::stringSetting(QString key) {
	return getSetting(key).toString().replace("\\=","=");
}

bool Core::boolSetting(QString key) {
	return getSetting(key).toBool();
}

QVariant Core::getSetting(QString key) {
	QVariant value = settings->value(key, "UNDEFINED");
	if(value.toString() == "UNDEFINED") {
		Debug::error("[Core] The Core Setting %1 is not defined!", key);
		return "UNDEFINED";
	} else {
		return value;
	}
}


