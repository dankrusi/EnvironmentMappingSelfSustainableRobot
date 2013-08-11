/*
 *  SplineNavigation.h
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

#ifndef SPLINENAVIGATION_H_
#define SPLINENAVIGATION_H_

#include "Navigation.h"

class SplineNavigation : public Navigation {

	Q_OBJECT;

#define NavNodeNotFound -1;

public:
	Spline1D *navSplineX;
	Spline1D *navSplineY;

	Spline1D *wheelLeftSplineX;
	Spline1D *wheelLeftSplineY;

	Spline1D *wheelRightSplineX;
	Spline1D *wheelRightSplineY;

private:
	double wheelOffset; 				// B value
	double targetSpeed; 				// Speed, used for determining distance between wheel spline nodes...
	int interval; 						// Interval of getWheelSpeed etc...
	int tickOffset;						// Offset used for getting the current node based on tick, used for navigation a spline not at beginning
	int intervalFlexibility;			// The amount of difference between the created spline interval and real interval allowed before a rebuild is done
	QTime lastGetWheelSpeedTimestamp;
	bool dynamicIntervalEnabled;		// When true, the interval is dynamically calculated and the spline automatically rebuilt to match the interval
	bool dynamicSplineNodeAdjust;		// When true, wheel spline nodes are automatically adjusted to the robots current local position
	bool magnetTrackingEnabled;

public:
	SplineNavigation(Core *core);
	virtual ~SplineNavigation();
	virtual void clearNavPoints();
	virtual Vector2D getWheelSpeed(int tick, int interval);
	double getAngleForSplineAlignment();
	void setTickOffset(int tick);
	int getNavNodeForTick(int tick);
	double getDistanceBetweenNavNodes(int node1, int node2);
	double getCurveLengthOfNode(int node, double interpolations);
	Vector2D getLeftWheelDisplacement(int node, double t);
	Vector2D getRightWheelDisplacement(int node, double t);
	Trafo2D getWheelTransformation(int node, double t);

protected:
	virtual void processAddNavPoint(Vector2D point);

private:
	void calculateLeftRightWheelSpline();
};

#endif /* SPLINENAVIGATION_H_ */
