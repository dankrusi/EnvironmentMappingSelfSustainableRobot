/*
 * CoreFactory.h
 *
 *  Created on: Mar 29, 2009
 *      Author: dankrusi
 */

#ifndef COREFACTORY_H_
#define COREFACTORY_H_

#include <QObject>
#include <QString>

class Core;
class COIL;
class Tracker;
class MovementTracker;
class Navigation;
class TaskManager;
class Controller;
class RemoteInterface;
class Watchdog;

class CoreFactory {

public:
	static COIL* createCOIL(Core *core, QString name);
	static Tracker* createTracker(Core *core, QString name);
	static MovementTracker* createMovementTracker(Core *core, QString name);
	static Navigation* createNavigation(Core *core, QString name);
	static TaskManager* createTaskManager(Core *core);
	static Controller* createController(Core *core, QString name);
	static RemoteInterface* createRemoteInterface(Core *core);
	static Watchdog* createWatchdog(Core *core);
};

#endif /* COREFACTORY_H_ */
