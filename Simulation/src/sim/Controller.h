#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <QObject>

#include "../object/Object.h"

class Simulation;

class Controller : public QObject {
	
	Q_OBJECT
	
public:
	Simulation	*simulation;
	Object 		*object;
	
public:
	Controller(Simulation *simulation, Object *object) { this->simulation = simulation; this->object = object; };
	virtual ~Controller() {};
	virtual void process() = 0;
};

#endif /*CONTROLLER_H_*/
