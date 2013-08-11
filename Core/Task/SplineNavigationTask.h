/*
 *  SplineNavigationTask.h
 *
 *  Spline navigation is very trivial. All it does is drive through all the
 *  navigational points contained in the Navigation component in the form
 *  of a spline by calling the Navigation methods getWheelSpeed(tick,interval).
 *  There is some additional functionality added, such as aligning the robot
 *  with the spline before navigation, and resetting the navigation points when
 *  finished.
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

#ifndef SPLINENAVIGATIONTASK_H_
#define SPLINENAVIGATIONTASK_H_

#include "Task.h"

class SplineNavigationTask : public Task  {

	Q_OBJECT

	public:
		int speed;
		int tick;

	public:
		SplineNavigationTask(Core *core, int speed, TaskPriority priority = Task::Normal);
		virtual ~SplineNavigationTask();
		virtual void process();
		virtual QString description();
};

#endif /* SPLINENAVIGATIONTASK_H_ */
