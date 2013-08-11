/*
 *  SingleTracker.cpp
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


#include "SingleTracker.h"


SingleTracker::SingleTracker(Core *core) : Tracker("Single", core) {
	selectedMovementTracker = NULL;
}

SingleTracker::~SingleTracker(){

}


Trafo2D SingleTracker::getTransformation(){
	Trafo2D retVal;
	lock.lockForRead();
	{
		if(selectedMovementTracker) {
			retVal = selectedMovementTracker->getTransformation();
		}
	}
	lock.unlock();
	return retVal;
}

long SingleTracker::getTotalDistance(){
	long retVal = 0;
	lock.lockForRead();
	{
		if(selectedMovementTracker) {
			retVal = selectedMovementTracker->getTotalDistance();
		}
	}
	lock.unlock();
	return retVal;
}

long SingleTracker::getTotalAngle(){
	long retVal = 0;
	lock.lockForRead();
	{
		if(selectedMovementTracker) {
			retVal = selectedMovementTracker->getTotalAngle();
		}
	}
	lock.unlock();
	return retVal;
}

void SingleTracker::setSelectedMovementTracker(MovementTracker *movementTracker) {
	lock.lockForWrite();
	{
		selectedMovementTracker = movementTracker;
	}
	lock.unlock();
}

void SingleTracker::setSelectedMovementTracker(QString name) {
	// Lock here because we need the list to be locked while we traverse it to find our tracker,
	// thus we don't use the setSelectedMovementTracker(MovementTracker*) method...
	lock.lockForWrite();
	{
		for (int i = 0; i < trackers->count(); i++){
			if(trackers->at(i)->name == name) {
				selectedMovementTracker = trackers->at(i);
				break;
			}
		}
	}
	lock.unlock();
}

MovementTracker* SingleTracker::getSelectedMovementTracker() {
	// No lock necessary
	return selectedMovementTracker;
}

