/*
 *  AveragedTracker.cpp
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


#include "AveragedTracker.h"


AveragedTracker::AveragedTracker(Core *core) : Tracker("Averaged", core) {

}

AveragedTracker::~AveragedTracker(){

}


Trafo2D AveragedTracker::getTransformation(){
	Trafo2D retVal;
	lock.lockForRead();
	{
		if(trackers->count() > 0) {
			int totalWeight = 0;
			for(int i = 0; i < trackers->count(); i++){
				totalWeight += trackers->at(i)->getWeight();
			}
//			double n = (double)trackers->count();
			double x = 0.0;
			double y = 0.0;
			double angle = 0.0;
			for (int i = 0; i < trackers->count(); i++){
				for(int k = 0; k < trackers->at(i)->getWeight();k++){
					Vector2D temp = trackers->at(i)->translation();
					x += temp.x();
					y += temp.y();
	//				double a = trackers->at(i)->rotation();
	//				a = fmod(a,360);
	//				angle += a;
					angle += trackers->at(i)->rotation();
				}
			}
			retVal = Trafo2D::trans(x/totalWeight, y/totalWeight) * Trafo2D::rot(Rad(angle/totalWeight));
		}
	}
	lock.unlock();
	return retVal;
}

long AveragedTracker::getTotalDistance(){
	long retVal = 0;
	lock.lockForRead();
	{
		int totalWeight = 0;
		for(int i = 0; i < trackers->count(); i++){
			totalWeight += trackers->at(i)->getWeight();
		}
		long temp = 0;
		for (int i = 0; i < trackers->count(); i++){
			for(int k = 0; k < trackers->at(i)->getWeight();k++){
				temp += trackers->at(i)->getTotalDistance();
			}
		}
		retVal = temp / totalWeight;
	}
	lock.unlock();
	return retVal;
}

long AveragedTracker::getTotalAngle(){
	long retVal = 0;
	lock.lockForRead();
	{
		int totalWeight = 0;
		for(int i = 0; i < trackers->count(); i++){
			totalWeight += trackers->at(i)->getWeight();
		}
		long temp = 0;
		for (int i = 0; i < trackers->count(); i++){
			for(int k = 0; k < trackers->at(i)->getWeight();k++){
				temp += trackers->at(i)->getTotalAngle();
			}
		}
		retVal = temp / totalWeight;
	}
	lock.unlock();
	return retVal;
}

