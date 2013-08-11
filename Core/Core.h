/*
 *  Core.h
 *
 *  Any application which interfaces with emss must initialize the Core object.
 *  The allocation and initialization of the Core class just allocates the data
 *  tier, in turn creating an empty environment and unknown state of the world,
 *  disconnected from the hardware. To use the emss Core and communicate with
 *  hardware, the application must call connect(controller,tracker,port) with
 *  the desired parameters on the Core object, which in turn creates all the
 *  connections between software and hardware, as well as lines of communications
 *  between the appropriate components. When finished, the application is
 *  responsible for calling disconnect() and deleting the Core object.
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

#ifndef CORE_H_
#define CORE_H_

#include <QObject>
#include <QString>
#include <QSettings>

// Forwards declarations
class COIL;

class GridMap;
class HeatMap;
class TerrainMap;
class ObjectMap;
class PhysicalMap;
class StructureMap;
class FadingCollisionMap;

class RobotObject;
class NavPathObject;
class NavigationObject;
class MarkerObject;

class Tracker;

class Controller;

class Navigation;

class Task;
class TaskManager;

class RemoteInterface;

class Watchdog;

class Joystick2D;

class Core : public QObject {

	Q_OBJECT

public:
	// Enums
	enum AreaType { AreaTypeCollision, AreaTypeOpen, AreaTypeUnknown };

public:
	// Core Instance Settings
	bool	connected;
	long 	scale;
	long 	mapWidth;
	long 	mapHeight;

	// Connection settings
	QString connectionController;
	QString connectionTracker;
	QString connectionNavigation;
	QString connectionSerialPort;
	bool	connectionSafeMode;
	bool	connectionEmulation;

public:
	// Core Modules and components

	COIL *coil;

	GridMap *gridMap;
	HeatMap *heatMap;
	TerrainMap *terrainMap;
	ObjectMap *objectMap;
	PhysicalMap *physicalMap;
	StructureMap *structureMap;
	FadingCollisionMap *fadingCollisionMap;

	NavigationObject *navigationObject;
	MarkerObject *markerObject;

	Tracker *tracker;
	Controller *controller;

	Navigation *navigation;

	TaskManager *taskManager;

	RemoteInterface *remoteInterface;

	Watchdog *watchdog;

	QSettings *settings;

	Joystick2D *joystick;

public:
	Core();
	virtual ~Core();
	bool connect(QString strController, QString strTracker, QString strNavigation, QString strSerialPort, bool safeMode, bool emulation);
	bool isConnected();
	long mmToPixels(long mm);
	long pixelsTomm(long pixels);
	void addTask(Task *task);
	void initMapsAndObjects();
	int intSetting(QString key);
	long longSetting(QString key);
	double doubleSetting(QString key);
	QString stringSetting(QString key);
	bool boolSetting(QString key);
	QVariant getSetting(QString key);

public slots:
	void robotMoved(long x,long y, double rotation);
	bool disconnect();
	void run();
	void stop();
	void abort(bool killThreads = false);
	void reset();

signals:
	void focusOnPoint(long x, long y);
	void coreReset();

};

#endif /* CORE_H_ */
