#ifndef PERSONCONTROLLER_H_
#define PERSONCONTROLLER_H_

#include "../sim/Controller.h"

#include "../object/PersonObject.h"

class PersonController : public Controller
{
	
public:
	double direction;
	double moveSpeed;
	double rotateSpeed;
	
public:
	PersonController(Simulation *simulation, PersonObject *object, double moveSpeed, double rotateSpeed) : Controller(simulation, object) { 
		this->direction = object->rotation; 
		this->rotateSpeed = rotateSpeed;
		this->moveSpeed = moveSpeed; 
	};
	virtual ~PersonController() {};
	virtual void process();
};

#endif /*PERSONCONTROLLER_H_*/
