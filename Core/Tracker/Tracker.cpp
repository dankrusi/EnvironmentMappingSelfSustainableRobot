/*
 *  Tracker.cpp
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


#include "Tracker.h"

#include "../Library/Debug.h"

Tracker::Tracker(QString name, Core *core) : CoreObject(name, core) {
	trackers = new QList<MovementTracker *>();

	Debug::print("[Tracker] %1 Tracker loaded", name);
}

Tracker::~Tracker(){
	//TODO: refactor to cleanup method
	for (int i = 0; i < trackers->count(); i++){
		delete trackers->at(i);
	}
	delete trackers;
}

void Tracker::connectController(QObject *target) {
	connect(target, SIGNAL(signalMovedDistance(double)), this, SLOT(registerMovedDistance(double)));
	connect(target, SIGNAL(signalChangedAngle(double)), this, SLOT(registerChangedAngle(double)));
	connect(target, SIGNAL(signalChangedWheelSpeed(int, int)), this, SLOT(registerChangedWheelSpeed(int, int)));
}

void Tracker::addMovementTracker(MovementTracker *movemenTracker){
	lock.lockForWrite();
	{
		trackers->append(movemenTracker);
	}
	Debug::print("[Tracker] added %1 Movement Tracker", movemenTracker->name);
	lock.unlock();
}

long Tracker::getX(){
	return getTransformation().trans().x();
}

long Tracker::getY(){
	return getTransformation().trans().y();
}

double Tracker::getRotation(){
	return Deg(getTransformation().angle());
}

Vector2D Tracker::getTranslation() {
	return getTransformation().trans();
}

Trafo2D Tracker::getWheelLeftTransformation() {
	return getTransformation() * Trafo2D::trans(-core->doubleSetting("Robot_WheelOffset_mm"), 0);
}


Trafo2D Tracker::getWheelRightTransformation() {
	return getTransformation() * Trafo2D::trans(+core->doubleSetting("Robot_WheelOffset_mm"), 0);
}


void Tracker::registerMovedDistance(double distance){
	lock.lockForRead();
	{
		for (int i = 0; i < trackers->count(); i++){
			trackers->at(i)->registerMovedDistance(distance);
		}
	}
	lock.unlock();
	emit moved(this->getX(), this->getY(), this->getRotation());
}

void Tracker::registerChangedAngle(double angle){
	lock.lockForRead();
	{
		for (int i = 0; i < trackers->count(); i++){
			trackers->at(i)->registerChangedAngle(angle);
		}
	}
	lock.unlock();
	emit moved(this->getX(), this->getY(), this->getRotation());
}

void Tracker::registerChangedWheelSpeed(int leftSpeed, int rightSpeed){
	lock.lockForRead();
	{
		for (int i = 0; i < trackers->count(); i++){
			trackers->at(i)->registerChangedWheelSpeed(leftSpeed, rightSpeed);
		}
	}
	lock.unlock();
	emit moved(this->getX(), this->getY(), this->getRotation());
}

void Tracker::setTransformation(Trafo2D t){
	lock.lockForRead();
	{
		for (int i = 0; i < trackers->count(); i++){
			trackers->at(i)->setTransformation(t);
		}
	}
	lock.unlock();
}


