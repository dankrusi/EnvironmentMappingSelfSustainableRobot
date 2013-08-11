#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <QObject>
#include <QList>
#include <QTimer>
#include <QTime>

#include "../map/PhysicalMap.h"
#include "../map/HeatMap.h"
#include "../map/GridMap.h"

#include "../object/RobotObject.h"
#include "../object/PersonObject.h"
#include "../object/HeatMapObject.h"

#include "../sim/Controller.h"
#include "../sim/PersonController.h"

class Simulation : public QObject
{
	Q_OBJECT
	
public:
	
	QTimer 				*timer;
	
	PhysicalMap 		*physicalMap;
	HeatMap 			*heatMap;
	GridMap 			*gridMap;
	
	RobotObject			*physicalRobot;
	RobotObject			*virtualRobot;
	
	QList<Controller*> 	*controllers;
	
public:
	Simulation();
	virtual ~Simulation();
	void init();
	void cleanup();
	void restart();
	void start() { timer->start(); };
	void stop() { timer->stop(); };
	
public slots:
	void processControllers();
	
signals:
	void repaint();
	
};

#endif /*SIMULATION_H_*/
