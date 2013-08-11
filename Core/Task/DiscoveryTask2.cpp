/*
 *  DiscoveryTask.cpp
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

#include <vector>
#include <cmath>


#include "DiscoveryTask2.h"

#include "../Controller/EmssController.h"
#include "../Tracker/MovementTracker.h"
#include "../Map/HeatMap.h"
#include "../Map/PhysicalMap.h"
#include "../Map/TerrainMap.h"
#include "../Map/StructureMap.h"

#include "../Navigation/Navigation.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"
#include "../Navigation/Weight/OrientationWeight.h"
#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/SystemOfWeightsNavigation.h"
#include "../Navigation/SplineNavigation.h"


DiscoveryTask2::DiscoveryTask2(Core *core, TaskPriority priority) :
	Task("Discovery", core, core->intSetting("Task_DiscoveryTask_Interval"), priority) {

	gridSize = 100;
	distanceBetweenNavPoints = 500;
	tick = 0;
}

DiscoveryTask2::~DiscoveryTask2() {

}

void DiscoveryTask2::preProcess() {
	// Check navigation module
	if(  core->navigation->name == "System Of Weights"){
		((SystemOfWeightsNavigation*)core->navigation)->removeAllWeights();
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new FullSpeedWeight(core));
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new OrientationWeight(core));
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new CollisionAvoidanceWeight(core));
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new AccelerationFilterWeight(core));
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new ControllerSpeedWeight(core));
	}
	else if (core->navigation->name == "Spline"){
		// using SplineNavigation
	}

	// Add all points inside the strcutureMap in a list
	int width = core->heatMap->width();
	int height = core->heatMap->height();
	if (core->structureMap->isFinish()){
		for(int x = 0; x < width; x+= gridSize){
			for (int y = 0; y < height; y+= gridSize){
				if (core->structureMap->isPointOutside(Vector2D(x,y)) == false){
					pointsInStructureMap.push_back(Vector2D(x,y));
				}
			}
		}
	}

	Debug::print("[DiscoveryTask2] points in structure map: %1", pointsInStructureMap.count());
}

void DiscoveryTask2::process() {

	// Check if the whole map is explored
	if (core->navigation->isAtLastNavPoint()) {

		if (isExplored() == true) {
			core->controller->setWheelSpeed(0, 0);
			Task::status = Task::Finished;
			Debug::print("[DiscoveryTask] DiscoveryTask finished");
			return;
		} else {
			Debug::print("[DiscoveryTask]Add new TerrainCutPoints");
			core->navigation->clearNavPoints();
			discover();
			tick = 0;
			return;
		}

	}

	// Align the robot and then crawl the spline...
	Vector2D wheelSpeed = core->navigation->getWheelSpeed(tick, interval);
	core->controller->setWheelSpeed((short) wheelSpeed.x(), (short) wheelSpeed.y());
	tick++;
}

QString DiscoveryTask2::description() {
	return QString("Name:\t%1").arg(name);
}

void DiscoveryTask2::discover() {

	QList<Vector2D> unknownAreaPoints;

	// Search all points in the pointsInStructureMap which are unknown
	for(int i = 0; i < pointsInStructureMap.count(); i++){
		if (core->heatMap->getChannelValue(HeatMap::OpenAreaChannel, pointsInStructureMap.at(i).x(), pointsInStructureMap.at(i).y()) > 0.0 || core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, pointsInStructureMap.at(i).x(), pointsInStructureMap.at(i).y()) > 0.0) {
			// Known Area
		} else {
			// Unknown Area
			int range = 10;
			bool knownAreaFoundInNeigbourhood = false;
			for(int x = -range; x <= range; x++ ){
				for(int y = -range; y <= range; y++ ){
					if (core->heatMap->getChannelValue(HeatMap::OpenAreaChannel, pointsInStructureMap.at(i).x() + x, pointsInStructureMap.at(i).y() +y) > 0.0 || core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, pointsInStructureMap.at(i).x() + x, pointsInStructureMap.at(i).y() + y) > 0.0){
						knownAreaFoundInNeigbourhood = true;
					}
				}
			}

			if(knownAreaFoundInNeigbourhood == false){
				unknownAreaPoints.push_back(Vector2L(pointsInStructureMap.at(i).x(), pointsInStructureMap.at(i).y()));
			}
		}
	}

	// Build a path as long as possible trough unknown points with a given distance between them
	if (unknownAreaPoints.count() > 0){
		QList<Vector2D> tempList;
		int minDistance = INT_MAX;
		int position = 0;
		for(int i = 0; i < unknownAreaPoints.count(); i++){
			int temp = dist(unknownAreaPoints.at(i), core->tracker->getTranslation());
			if (temp < minDistance){
				position = i;
				minDistance = temp;
			}
		}
		tempList.push_back(unknownAreaPoints.at(position));
		foreach(Vector2D p1, unknownAreaPoints){
			if (dist(tempList.last(), p1) == distanceBetweenNavPoints){
				bool isNearAnOtherPoint = false;
				foreach(Vector2D p2, tempList){
					if (dist(p1,p2) < distanceBetweenNavPoints){
						isNearAnOtherPoint = true;
					}
				}
				if (isNearAnOtherPoint == false){
					tempList.push_back(p1);
				}
			}
		}
		foreach(Vector2L p, tempList){
			this->core->navigation->addNavPoint(p);
		}
	}
}

bool DiscoveryTask2::isExplored() {
	if(pointsInStructureMap.count() < 1) return false;

	// Count the discovered points in a map
	int explored = 0;
	foreach(Vector2D p,pointsInStructureMap){
		if ((core->heatMap->getChannelValue(HeatMap::OpenAreaChannel, p.x(), p.y()) > 0.0 || core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, p.x(), p.x()) > 0.0)){
			explored++;
		}
	}

	// The map is discoverd if the given percent of the map is discovered
	double percentOftheMapMustBeDisovered = 0.95;
	if(((double)explored) / pointsInStructureMap.count() > percentOftheMapMustBeDisovered){
		return true;
	}

	return false;
}
