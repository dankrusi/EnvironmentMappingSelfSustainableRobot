#include "Simulation.h"

Simulation::Simulation()
{
	init();
	timer->start();
}

void Simulation::init() {
	// Init maps
	physicalMap = new PhysicalMap(":/maps/physical/test1.bmp");
	heatMap = new HeatMap(physicalMap->width, physicalMap->height);
	gridMap = new GridMap(50);

	// Init robots and other objects
	physicalRobot = new RobotObject(physicalMap, 100.0, 100.0);
	virtualRobot = new RobotObject(physicalMap, 100.0, 100.0);
	
	// Init controllers
	controllers = new QList<Controller*>();
	
	// Test area - TODO
//	PersonObject *testPerson = new PersonObject(physicalMap, 371.0, 181.0); // TODO
//	controllers->append(new PersonController(this, testPerson, 5.0, 10.0));
	
	// Init timers
	timer = new QTimer(this);
	timer->setInterval(50);
	
	// Miscellaneous
	qsrand(QTime::currentTime().msec());
	
	// Connections
	connect(timer, SIGNAL(timeout()), this, SLOT(processControllers()));
}

void Simulation::cleanup() {
	delete physicalMap;
	delete heatMap;
	delete gridMap;
	delete controllers;
	delete timer;
}

void Simulation::restart() {
	timer->stop();
	cleanup();
	init();
	timer->start();
}
	
Simulation::~Simulation()
{
	timer->stop();
	cleanup();
}

void Simulation::processControllers() {
	// Process all registerd controllers and signal a repaint
	for(int i = 0; i < controllers->count(); i++) {
		controllers->at(i)->process();
	}
	emit repaint();
}
