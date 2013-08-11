/*
 *  TestMoveTask.cpp
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

#include "TestMoveTask.h"

#include "../Controller/EmssController.h"
#include "../Library/Math.h"
#include "../Library/Debug.h"
#include "../Navigation/Navigation.h"
#include "../Tracker/MovementTracker.h"

#include "../Navigation/Weight/WallFollowerWeight.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"
#include "../Navigation/Weight/OrientationWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/JoystickWeight.h"

TestMoveTask::TestMoveTask(Core *core, QString move, TaskPriority priority) :
	MoveTask("Test Move", core, core->intSetting("Task_TestMoveTask_Interval"), priority) {
	this->move = move;
	tick = 0;
}

TestMoveTask::~TestMoveTask() {

}

void TestMoveTask::preProcess() {

	if(  core->navigation->name == "System Of Weights" && this->move == "Accuracy Test"){
		((SystemOfWeightsNavigation *)core->navigation)->removeAllWeights();
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new FullSpeedWeight(core));
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new OrientationWeight(core));
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new AccelerationFilterWeight(core));
		((SystemOfWeightsNavigation *)core->navigation)->addWeight(new ControllerSpeedWeight(core));

		// Set navigation points
		Vector2D robot = core->tracker->getTranslation();
//		core->navigation->addNavPoint(robot.x(), robot.y() + 500);
//		core->navigation->addNavPoint(robot.x() - 1000, robot.y() + 300);
//		core->navigation->addNavPoint(robot.x() - 1000, robot.y() + 800);
//		core->navigation->addNavPoint(robot.x() - 500, robot.y() + 800);
//		core->navigation->addNavPoint(robot.x() + 500, robot.y() + 1800);
//		core->navigation->addNavPoint(robot.x() + 1000, robot.y() + 900);
//		core->navigation->addNavPoint(robot.x() + 1500, robot.y());
//		core->navigation->addNavPoint(robot.x() + 1000, robot.y() - 100);
//		core->navigation->addNavPoint(robot.x() + 500, robot.y() -300);
//		core->navigation->addNavPoint(robot.x(), robot.y());

		core->navigation->addNavPoint(robot.x(), robot.y() + 500);
		core->navigation->addNavPoint(robot.x() - 1000, robot.y() + 1000);
		core->navigation->addNavPoint(robot.x() + 500, robot.y() + 1500);
		core->navigation->addNavPoint(robot.x() - 500, robot.y() + 2000);
		core->navigation->addNavPoint(robot.x() + 500, robot.y() + 2500);
		core->navigation->addNavPoint(robot.x() - 1000, robot.y() + 3000);
		core->navigation->addNavPoint(robot.x() + 500, robot.y() + 3500);
		core->navigation->addNavPoint(robot.x() - 500, robot.y() + 4000);
		core->navigation->addNavPoint(robot.x() + 500, robot.y() + 4500);
		core->navigation->addNavPoint(robot.x()- 1000, robot.y() + 5000);
	}
	else if (core->navigation->name == "Spline" && this->move == "Accuracy Test"){
		Debug::warning("This navigation module is not supported");
		this->status = Interrupted;
	}
}

void TestMoveTask::process() {

	int DISTANCE = 1000; // 100 cm

	if(move == "Square") {

		MoveTask::turn(90);
		MoveTask::move(DISTANCE);
		MoveTask::turn(90);
		MoveTask::move(DISTANCE);
		MoveTask::turn(90);
		MoveTask::move(DISTANCE);
		MoveTask::turn(90);
		MoveTask::move(DISTANCE);
		status = Task::Finished;

	} else if(move == "Rotate 90") {

		MoveTask::turn(90);
		MoveTask::turn(-90);
		status = Task::Finished;

	} else if(move == "Rotate 360") {

		MoveTask::turn(360);
		MoveTask::turn(-360);
		status = Task::Finished;

	} else if(move == "Triangle") {

		MoveTask::turn(60);
		MoveTask::move(DISTANCE);
		MoveTask::turn(120);
		MoveTask::move(DISTANCE);
		MoveTask::turn(120);
		MoveTask::move(DISTANCE);
		status = Task::Finished;

	} else if(move == "Straight") {

		MoveTask::move(DISTANCE, 1);
		MoveTask::move(DISTANCE, -1);
		status = Task::Finished;

	} else if(move == "Circle") {

		double radius = 100;
		double b = 20;
		double len = 10;
		double circ = 20;
		double growth = 1.0;
		double vl = 0.0;
		double vr = 0.0;
		int t = tick;

		// Body path
		{
			double f = 1+((double)Rad(t)*growth)-(double)Rad(t);
			double px = std::sin(Rad(t))*radius*f;
			double py = std::cos(Rad(t))*radius*f;
		}

		// Outer path
		{
			double f = 1+((double)Rad(t)*growth)-(double)Rad(t);
			double px = std::sin(Rad(t))*(radius*f + b);
			double py = std::cos(Rad(t))*(radius*f + b);
			double dx = std::cos(Rad(t))*(radius*f + b);
			double dy = -1.0*std::sin(Rad(t))*(radius*f + b);
			double d = std::sqrt(dx*dx + dy*dy);
			vl = d / circ;
		}

		// Inner path
		{
			double f = 1+((double)Rad(t)*growth)-(double)Rad(t);
			double px = std::sin(Rad(t))*(radius*f - b);
			double py = std::cos(Rad(t))*(radius*f - b);
			double dx = std::cos(Rad(t))*(radius*f - b);
			double dy = -1.0*std::sin(Rad(t))*(radius*f - b);
			double d = std::sqrt(dx*dx + dy*dy);
			vr = d / circ;
		}

		vl *= 10.0;
		vr *= 10.0;

		core->controller->setWheelSpeed((short)vl,(short)vr);

		if(tick > 360) {
			status = Task::Finished;
			core->controller->setWheelSpeed(0,0);
		}

	} else if(move == "Vector Circle") {

		double radius = 100;
		double b = 20;
		double circ = 20;
		double speed = 20;

		Vector2D c(std::sin(Rad(tick))*radius, std::cos(Rad(tick))*radius);
		Vector2D c1(std::cos(Rad(tick))*radius, -1.0*std::sin(Rad(tick))*radius);
		Vector2D c2(-1.0 * std::sin(Rad(tick))*radius, -1.0*std::cos(Rad(tick))*radius);
		//c1 = speed * c1;

		Vector2D n = Trafo2D::rot(Rad(90)) * c1;

		Vector2D l = c + (b * (n/(Sqrt(n.x()*n.x()+n.y()*n.y()))));
		Vector2D r = c - (b * (n/(Sqrt(n.x()*n.x()+n.y()*n.y()))));

		Vector2D n1 = Trafo2D::rot(Rad(90)) * c2;

		Vector2D l1 = c1 + (b * (n1/(Sqrt(n.x()*n.x()+n.y()*n.y()))));
		Vector2D r1 = c1 - (b * (n1/(Sqrt(n.x()*n.x()+n.y()*n.y()))));

		double vl = (Sqrt(l1.x()*l1.x() + l1.y() * l1.y()))/circ;
		double vr = (Sqrt(r1.x()*r1.x() + r1.y() * r1.y()))/circ;

		vl *= speed;
		vr *= speed;

		core->controller->setWheelSpeed((short)vl,(short)vr);

		if(tick > 360) {
			status = Task::Finished;
			core->controller->setWheelSpeed(0,0);
		}
	} else if(move == "Accuracy Test") {

		Vector2D wheelSpeed = core->navigation->getWheelSpeed(0,0);

		if(core->navigation->isAtLastNavPoint()){
			wheelSpeed = Vector2D(0,0);
			this->status = Finished;
		}

		core->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

	}

	tick++;

}

QString TestMoveTask::description() {
	return QString("Move:\t%1\nSpeed:\t%2").arg(move).arg(speed);
}

