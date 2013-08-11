/*
 *  DiscoveryTask.h
 *
 *  This is a simple implementation of the classic robot discovery algorithm.
 *  The robot moves straight until it reaches a collision. Upon reaching a
 *  collision it rotates until it is free to move. This algorithm works well
 *  for certain environments for finding the bounds (i.e. walls), but quickly
 *  finds itself stuck in a loop of a specific path.
 *
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

#ifndef DISCOVERYTASK_H_
#define DISCOVERYTASK_H_

#include "Task.h"
#include "../Library/TerrainPoint.h"
#include "../Library/Math.h"

class DiscoveryTask : public Task {

	Q_OBJECT

public:
	int speed;
	enum { Discovered, Obstacle, Unreachable, Unknown} status;

private:
	bool running;
	TerrainPointT<int> nextPoint;
	int tick;
	bool lastProcessWasCollision;
	bool collisionInCriticalArea;
	QList<Vector2D> pointsInStructureMap;

public:
	DiscoveryTask(Core *core, int speed, TaskPriority priority = Task::Normal);
	virtual ~DiscoveryTask();
	virtual void preProcess();
	virtual void process();
	virtual QString description();

private:
	void addNewTerrainCutPoints();
	bool isExplored();
};

#endif /* DISCOVERYTASK_H_ */
