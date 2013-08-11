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


#include "DiscoveryTask.h"

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


DiscoveryTask::DiscoveryTask(Core *core, int speed, TaskPriority priority) :
	Task("Discovery", core, core->intSetting("Task_DiscoveryTask_Interval"), priority) {

	this->speed = speed;
	tick = 0;
	lastProcessWasCollision = false;
	collisionInCriticalArea = false;
	running = false;
}

DiscoveryTask::~DiscoveryTask() {

}

void DiscoveryTask::preProcess() {
	int width = core->heatMap->width();
	int height = core->heatMap->height();
	if (core->structureMap->isFinish()){
		for(int x = 0; x < width; x+= 100){
			for (int y = 0; y < height; y+= 100){
				if (core->structureMap->isPointOutside(Vector2D(x,y)) == false){
					pointsInStructureMap.push_back(Vector2D(x,y));
				}
			}
		}
	}
}

void DiscoveryTask::process() {

	// Check navigation module
	if(  core->navigation->name == "System Of Weights" && !running){
		((SystemOfWeightsNavigation*)core->navigation)->removeAllWeights();
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new FullSpeedWeight(core));
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new OrientationWeight(core));
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new CollisionAvoidanceWeight(core));
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new AccelerationFilterWeight(core));
		((SystemOfWeightsNavigation*)core->navigation)->addWeight(new ControllerSpeedWeight(core));
		running = true;
	}
	else if (core->navigation->name == "Spline"){
		// using SplineNavigation
	}
	else if (!running){
		Debug::warning("[DiscoveryTask] Navigation module is not of type Spline!");
		Task::status = Task::Finished;
		return;
	}


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
			addNewTerrainCutPoints();
			tick = 0;
			return;
		}

	}

	// Look ahead if it is good to move there using a sweep across a start and end angle...
//	double heat = 0.0;
//	for (int i = 0; i < core->longSetting("Task_DiscoveryTask_Range_mm"); i += 10) {
//		for (int angle = -(core->longSetting("Task_DiscoveryTask_SweepAngle") / 2); angle <= (core->longSetting("Task_DiscoveryTask_SweepAngle") / 2); angle++) {
//			Trafo2D checkpoint = core->movementTracker->transformation * Trafo2D::rot(-Rad(angle)) * Trafo2D::trans(0, i);
//			heat += core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, (long) ((checkpoint.trans().x()) / core->scale), (long) ((checkpoint.trans().y()) / core->scale));
//			if (i < core->longSetting("Task_DiscoveryTask_CriticalAreaDistance_mm") && heat > 0) {
//				collisionInCriticalArea = true;
//			}
//		}
//	}
//
//	// Safe to move?
//	if (heat > 0.0 && lastProcessWasCollision == false) {
//
//		for (int angle = -(core->longSetting("Task_DiscoveryTask_SweepAngle") / 4); angle <= (core->longSetting("Task_DiscoveryTask_SweepAngle") / 4); angle++) {
//			Trafo2D checkpoint = core->movementTracker->transformation * Trafo2D::rot(-Rad(angle)) * Trafo2D::trans(0, 2000);
//			core->heatMap->registerHeat(HeatMap::CollisionAreaChannel, (long) ((checkpoint.trans().x()) / core->scale), (long) ((checkpoint.trans().y()) / core->scale));
//		}
//
//		// is the collision in the critical area?
//		if (collisionInCriticalArea == false) {
//			//Debug::print("\t[Start] Collision");
////			if (core->navigation->currentNavPoint < core->navigation->getNavPoints()->count()-2){
////				core->navigation->currentNavPoint++;
////				//if(core->navigation->isAtLastNavPoint()) return;
////			}
////			else {
//				core->navigation->clearNavPoints();
//				addNewTerrainCutPoints();
//				tick = 0;
//				lastProcessWasCollision = true;
////			}
//
//			//Debug::print("\t[Start] No Collision");
//		} else {
//			//Debug::print("\t[Start] Turn away from Collision");
//			// turn away from the collision
//			((EmssController*) core->controller)->turn(90,(int) core->doubleSetting("Navigation_SplineNavigation_TargetSpeed_mmps"));
//			core->navigation->clearNavPoints();
//			addNewTerrainCutPoints();
//			tick = 0;
//			lastProcessWasCollision = true;
//			collisionInCriticalArea = false;
//			//Debug::print("\t[End] Turn away from Collision");
//		}
//		//Debug::print("\t[End] Collision in Area?");
//	} else {
//		lastProcessWasCollision = false;
//	}
	// Align the robot and then crawl the spline...
	Vector2D wheelSpeed = core->navigation->getWheelSpeed(tick, interval);
	core->controller->setWheelSpeed((short) wheelSpeed.x(), (short) wheelSpeed.y());
	tick++;
}

QString DiscoveryTask::description() {
	return QString("Speed:\t%1").arg(speed);
}

void DiscoveryTask::addNewTerrainCutPoints() {

	//Debug::print("[Start] AddNewTerrainCutPoints");
	long RELATIVE_EXPLORATION_WIDTH = core->longSetting("Task_DiscoveryTask_RelativeExplorationWidth_mm");
	long RELATIVE_EXPLORATION_HEIGHT = core->longSetting("Task_DiscoveryTask_RelativeExplorationHeight_mm");
	long EXPLORATION_MAP_GRID_SIZE = core->longSetting("Task_DiscoveryTask_ExplorationMapGridSize");
	long TERRAIN_UNKNOWN_HEIGHT = core->longSetting("Task_DiscoveryTask_TerrainMapUnknownHeight");
	long TERRAIN_MAP_MAX_HEIGHT = core->longSetting("Task_DiscoveryTask_TerrainMapMaxHeight");
	long TERRAIN_CUT_LEVEL = core->longSetting("Task_DiscoveryTask_TerrainCutLevel");

	// Initialize variables
	long mapWidth = RELATIVE_EXPLORATION_WIDTH / EXPLORATION_MAP_GRID_SIZE;
	long mapHeight = RELATIVE_EXPLORATION_HEIGHT / EXPLORATION_MAP_GRID_SIZE;

	int heights[mapWidth][mapHeight];
	int explorationMap[mapWidth][mapHeight];

	long robotX = core->tracker->getX();
	long robotY = core->tracker->getY();

	// Fill array with default values
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight; y++) {
			heights[x][y] = TERRAIN_UNKNOWN_HEIGHT;
		}
	}
	//Debug::print("\t[Start] Fill exploration array");
	// Fill the exploration array
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight; y++) {
			//long heatMapX = (robotX - ((mapWidth/2) + x)*core->longSetting("Task_DiscoveryTask_ExplorationMapGridSize"))/core->scale;
			//long heatMapY = (robotY - ((mapHeight/2)+ y)*core->longSetting("Task_DiscoveryTask_ExplorationMapGridSize"))/core->scale;
			long heatMapX = robotX - RELATIVE_EXPLORATION_WIDTH / 2 + x * EXPLORATION_MAP_GRID_SIZE;
			long heatMapY = robotY - RELATIVE_EXPLORATION_HEIGHT / 2 + y * EXPLORATION_MAP_GRID_SIZE;
			if (core->heatMap->getChannelValue(HeatMap::OpenAreaChannel, heatMapX, heatMapY) > 0.0) {
				explorationMap[x][y] = Discovered;
			} else if (core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, heatMapX, heatMapY) > 0.0) {
				explorationMap[x][y] = Obstacle;
			} else if (core->structureMap->isPointOutside(Vector2D(heatMapX,heatMapY))){
				explorationMap[x][y] = Unreachable;
			}
			else {
				explorationMap[x][y] = Unknown;
			}
		}
	}
	//Debug::print("\t[End] Fill exploration array");

	int h;
	QQueue<TerrainPointT<int> > queue;
	queue.enqueue(TerrainPointT<int> (mapWidth / 2, mapHeight / 2, 0));

	//Debug::print("\t[Start] Calculating Heights");
	// calculate the heights
	while (!queue.isEmpty()) {
		TerrainPointT<int> p = queue.dequeue();

		if (p.height + 1 > TERRAIN_MAP_MAX_HEIGHT) {
			h = TERRAIN_MAP_MAX_HEIGHT;
		} else {
			h = p.height + 1;
		}

		for (int k = -1; k <= 1; k++) {
			for (int j = -1; j <= 1; j++) {
				if (p.x + k < mapWidth && p.x + k >= 0 && p.y + j < mapHeight && p.y + j >= 0 && (heights[p.x + k][p.y + j] > h || heights[p.x + k][p.y + j] == TERRAIN_UNKNOWN_HEIGHT)) {
					switch (explorationMap[p.x + k][p.y + j]) {
					case Obstacle:
						heights[p.x + k][p.y + j] = 0;
						queue.enqueue(TerrainPointT<int> (p.x + k, p.y + j, 0));
						break;
					case Unreachable:
					case Discovered:
						heights[p.x + k][p.y + j] = 0;
						queue.enqueue(TerrainPointT<int> (p.x + k, p.y + j, 0));
						break;
					case Unknown:
						heights[p.x + k][p.y + j] = h;
						queue.enqueue(TerrainPointT<int> (p.x + k, p.y + j, h));
						break;
					}
				}
			}
		}
	}
	//Debug::print("\t[End] Calculating Heights");

	std::vector<QPoint> terrainCutPoints;
	int cutLevel = TERRAIN_CUT_LEVEL;

	//Debug::print("\t[Start] Add the points");
	// Add the points with the terrain cut height to the spline
	while (terrainCutPoints.size() < 3) {
		for (int x = 0; x < mapWidth; x++) {
			for (int y = 0; y < mapHeight; y++) {
				//if(heights[x][y] <= 100)Debug::print("Height: %1, x: %2, y: %3", heights[x][y], (int)robotX - (mapWidth/2) + x,(int)robotY - (mapHeight/2) + y);
				if (heights[x][y] == cutLevel) {

					//terrainCutPoints.push_back(QPoint(robotX - (mapWidth/2) + x,robotY - (mapHeight/2) + y));
					long navPointX = robotX - RELATIVE_EXPLORATION_WIDTH / 2 + x * EXPLORATION_MAP_GRID_SIZE;
					long navPointY = robotY - RELATIVE_EXPLORATION_HEIGHT / 2 + y * EXPLORATION_MAP_GRID_SIZE;
					terrainCutPoints.push_back(QPoint(navPointX, navPointY));

					//this->core->navigation->addNavPoint(robotX - (mapWidth/2) + x, robotY - (mapHeight/2) + y);
					//std::cout << "X: " << robotX - (mapWidth/2) + x << " Y: " << robotY - (mapHeight/2) + y << std::endl;

				}
			}
		}
		cutLevel--;
	}
	//Debug::print("\t[End] Add the points");

	//Debug::print("\t[Start] Sort the points");
	// Sort the terrain cut points
	for (int i = 0; i < (int)terrainCutPoints.size(); i++) {

		int distance = INT_MAX;
		int nearestPoint = 0;
		for (int k = i + 1; k < (int)terrainCutPoints.size(); k++) {
			int temp = abs(terrainCutPoints[i].x() - terrainCutPoints[k].x()) + abs(terrainCutPoints[i].y() - terrainCutPoints[k].y());
			if (temp < distance) {
				distance = temp;
				nearestPoint = k;
			}
		}
		if (i != (int)terrainCutPoints.size() - 1) {
			QPoint temp = terrainCutPoints[i + 1];
			terrainCutPoints[i + 1] = terrainCutPoints[nearestPoint];
			terrainCutPoints[nearestPoint] = temp;
		}

	}
	//Debug::print("\t[End] Sort the points");

	//Debug::print("\t[Start] Add the points to navigation");
	// Add the terrain cut point to the navigation spline
	QPoint lastPointAdded;
	for (int i = 0; i < (int)terrainCutPoints.size(); i++) {
		//		// Take only the cut points with the distance CUT_POINT_DISTANCE from the last point
		if (i == 0) {
			this->core->navigation->addNavPoint(terrainCutPoints[i].x(), terrainCutPoints[i].y());
			lastPointAdded = terrainCutPoints[i];
		}
		if (i != 0 && i != (int)terrainCutPoints.size() - 1 && std::sqrt(std::pow(lastPointAdded.x() - terrainCutPoints[i].x(), 2.0) + std::pow(lastPointAdded.y() - terrainCutPoints[i].y(), 2.0)) > core->intSetting("Task_DiscoveryTask_TerrainCutPointDistance_mm")) {
			this->core->navigation->addNavPoint(terrainCutPoints[i].x(), terrainCutPoints[i].y());
			lastPointAdded = terrainCutPoints[i];
		}

	}
	//Debug::print("\t[End] Add the points to navigation");
	//Debug::print("\t[Start] Print terrain map");

	// Print the height matrix onto the terrain map for visual representation...
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight; y++) {
			double percent = heights[x][y] / 100.0;
			//percent = 1.0;
			long terrainMapX = robotX - RELATIVE_EXPLORATION_WIDTH / 2 + x * EXPLORATION_MAP_GRID_SIZE;
			long terrainMapY = robotY - RELATIVE_EXPLORATION_HEIGHT / 2 + y * EXPLORATION_MAP_GRID_SIZE;
			QColor color = Qt::white;
			//			if (explorationMap[x][y] == Discovered) color = Qt::green;
			//			else if (explorationMap[x][y] == Obstacle) color = Qt::red;
			core->terrainMap->setHeight(terrainMapX, terrainMapY, EXPLORATION_MAP_GRID_SIZE, EXPLORATION_MAP_GRID_SIZE, percent, color);
		}
	}
	//Debug::print("\t[End] Print terrain map");

	//Debug::print("[End] AddNewTerrainCutPoints");
}

bool DiscoveryTask::isExplored() {

	if(pointsInStructureMap.count() < 1) return false;

	int explored = 0;

	foreach(Vector2D p,pointsInStructureMap){
		if ((core->heatMap->getChannelValue(HeatMap::OpenAreaChannel, p.x(), p.y()) > 0.0 || core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, p.x(), p.x()) > 0.0)){
			explored++;
		}
	}

	std::cout << explored << " " << pointsInStructureMap.count() << " " << explored / pointsInStructureMap.count() << std::endl;

	if(explored / pointsInStructureMap.count() > 0.95){
		return true;
	}

	return false;
//	long RELATIVE_EXPLORATION_WIDTH = core->longSetting("Task_DiscoveryTask_RelativeExplorationWidth_mm");
//	long RELATIVE_EXPLORATION_HEIGHT = core->longSetting("Task_DiscoveryTask_RelativeExplorationHeight_mm");
//	long EXPLORATION_MAP_GRID_SIZE = core->longSetting("Task_DiscoveryTask_ExplorationMapGridSize");
//	long robotX = core->tracker->getX();
//	long robotY = core->tracker->getY();
//
//	bool retVal = true;
//
//	for (int x = 0; x < RELATIVE_EXPLORATION_WIDTH / EXPLORATION_MAP_GRID_SIZE; x++) {
//		for (int y = 0; y < RELATIVE_EXPLORATION_HEIGHT / EXPLORATION_MAP_GRID_SIZE; y++) {
//			long heatMapX = robotX - RELATIVE_EXPLORATION_WIDTH / 2 + x * EXPLORATION_MAP_GRID_SIZE;
//			long heatMapY = robotY - RELATIVE_EXPLORATION_HEIGHT / 2 + y * EXPLORATION_MAP_GRID_SIZE;
//
//			if ((core->heatMap->getChannelValue(HeatMap::OpenAreaChannel, heatMapX / core->scale, heatMapY / core->scale) > 0.0 || core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, heatMapX / core->scale, heatMapY / core->scale) > 0.0) == false) {
//				retVal = false;
//			}
//			else if (core->structureMap->isPointOutside(Vector2D(heatMapX,heatMapY)) == false){
//				retVal = false;
//			}
//		}
//	}


//	for (int x = 0; x < RELATIVE_EXPLORATION_WIDTH / EXPLORATION_MAP_GRID_SIZE; x++) {
//		for (int y = 0; y < RELATIVE_EXPLORATION_HEIGHT / EXPLORATION_MAP_GRID_SIZE; y++) {
//			long heatMapX = robotX - RELATIVE_EXPLORATION_WIDTH / 2 + x * EXPLORATION_MAP_GRID_SIZE;
//			long heatMapY = robotY - RELATIVE_EXPLORATION_HEIGHT / 2 + y * EXPLORATION_MAP_GRID_SIZE;
//			if (core->heatMap->getChannelValue(HeatMap::OpenAreaChannel, heatMapX / core->scale, heatMapY / core->scale) > 0.0 || core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, heatMapX / core->scale, heatMapY / core->scale) > 0.0) {
//			} else {
//				retVal = false;
//			}
//		}
//	}
	//Debug::print("\t[End] isExplored");
//	return retVal;
}
