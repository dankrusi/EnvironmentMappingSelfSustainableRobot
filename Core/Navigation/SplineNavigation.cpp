/*
 *  SplineNavigation.cpp
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

#include "SplineNavigation.h"

#include "../Core.h"
#include "../Library/Debug.h"
#include "../Tracker/Tracker.h"
#include "../Object/MarkerObject.h"

//BUG: The Stoer kernel does not work! Why do the first two points just not create a valid wheel spline? It just doesnt make sense!


SplineNavigation::SplineNavigation(Core *core) : Navigation("Spline", core) {

	// Init
	tickOffset = 0;
	currentNavPoint = NavNodeNotFound;

	// Init splines
	navSplineX = new Spline1D();
	navSplineY = new Spline1D();
	wheelLeftSplineX = new Spline1D();
	wheelLeftSplineY = new Spline1D();
	wheelRightSplineX = new Spline1D();
	wheelRightSplineY = new Spline1D();

	// Set correct kernel...
	SplineKernel splineKernelWheel = SplineKernelBruenner;
	if 			(core->stringSetting("Navigation_SplineNavigation_WheelSplineKernel") == "SplineKernelBruenner") 	splineKernelWheel = SplineKernelBruenner;
	else if 	(core->stringSetting("Navigation_SplineNavigation_WheelSplineKernel") == "SplineKernelNRNatural") 	splineKernelWheel = SplineKernelNRNatural;
	else if 	(core->stringSetting("Navigation_SplineNavigation_WheelSplineKernel") == "SplineKernelStoer") 		splineKernelWheel = SplineKernelStoer;
	SplineKernel splineKernelNavPoints = SplineKernelBruenner;
	if 			(core->stringSetting("Navigation_SplineNavigation_NavPointsSplineKernel") == "SplineKernelBruenner") 	splineKernelNavPoints = SplineKernelBruenner;
	else if 	(core->stringSetting("Navigation_SplineNavigation_NavPointsSplineKernel") == "SplineKernelNRNatural") 	splineKernelNavPoints = SplineKernelNRNatural;
	else if 	(core->stringSetting("Navigation_SplineNavigation_NavPointsSplineKernel") == "SplineKernelStoer") 		splineKernelNavPoints = SplineKernelStoer;
	navSplineX->setKernel(splineKernelNavPoints);
	navSplineY->setKernel(splineKernelNavPoints);
	wheelLeftSplineX->setKernel(splineKernelWheel);
	wheelLeftSplineY->setKernel(splineKernelWheel);
	wheelRightSplineX->setKernel(splineKernelWheel);
	wheelRightSplineY->setKernel(splineKernelWheel);

	// Init...
	wheelOffset = core->doubleSetting("Robot_WheelOffset_mm"); // Offset wheel 120mm
	targetSpeed = core->doubleSetting("Navigation_SplineNavigation_TargetSpeed_mmps"); // mm/s
	dynamicSplineNodeAdjust = core->boolSetting("Navigation_SplineNavigation_SplineNodeAdjust");
	dynamicIntervalEnabled = core->boolSetting("Navigation_SplineNavigation_DynamicIntervalEnabled");
	magnetTrackingEnabled = core->boolSetting("Navigation_SplineNavigation_MagnetTrackingEnabled");

	// Setup the interval mechanism
	if(dynamicIntervalEnabled == true) {

		// Dynamic interval
		interval = core->intSetting("Navigation_SplineNavigation_FixedInterval"); //ms
		intervalFlexibility = core->intSetting("Navigation_SplineNavigation_DynamicIntervalFlexibility_ms");
		lastGetWheelSpeedTimestamp = QTime();

	} else {

		// Fixed interval
		interval = core->intSetting("Navigation_SplineNavigation_FixedInterval"); //ms
	}

}

SplineNavigation::~SplineNavigation() {

	delete navSplineX;
	delete navSplineY;

	delete wheelLeftSplineX;
	delete wheelLeftSplineY;

	delete wheelRightSplineX;
	delete wheelRightSplineY;

}

void SplineNavigation::calculateLeftRightWheelSpline() {

	double wheelNodeDistanceBetweenNode = targetSpeed * ((double) interval / 1000.0);

	// Init and drop nodes
	wheelLeftSplineX->clearNodes();
	wheelLeftSplineY->clearNodes();
	wheelRightSplineX->clearNodes();
	wheelRightSplineY->clearNodes();

	// Calculate new nodes for wheel splines
	for (int navNode = 0; navNode < navSplineX->getNodeCount() - 1; navNode++) {

		// Get the distance between the two nav nodes
		double distanceBetweenNodes = getDistanceBetweenNavNodes(navNode, navNode+1);

		// Create the wheel nodes at equal distance between the two nodes
		for (int wheelNode = 0; wheelNode < (distanceBetweenNodes / wheelNodeDistanceBetweenNode); wheelNode++) {

			// Get the t value for the nav node, which is discretisized based on the wheel node distance between nodes
			double t = (double) wheelNode / (double) (distanceBetweenNodes / wheelNodeDistanceBetweenNode);

			// Get the node positions for both the left and right wheel splines
			Vector2D c(navSplineX->getValue(navNode, t), navSplineY->getValue(navNode, t));
			Vector2D c1(navSplineX->getFirstDerivative(navNode, t), navSplineY->getFirstDerivative(navNode, t));
			Vector2D v = c1 / norm(c1);
			Vector2D n = Vector2D(-v.y(), v.x());
			Vector2D l = c + (wheelOffset * n);
			Vector2D r = c - (wheelOffset * n);

			wheelLeftSplineX->addNode(l.x());
			wheelLeftSplineY->addNode(l.y());
			wheelRightSplineX->addNode(r.x());
			wheelRightSplineY->addNode(r.y());

			//Debug::print("n=%3 nlx=%1 wlx=%2", l.x(), wheelLeftSplineX->getValue(wheelNode, 0.0), wheelNode);
		}
	}
}

// Returns the rough angle in degrees the robot needs to turn in order to
// be properly aligned with the spline...
double SplineNavigation::getAngleForSplineAlignment() {
	// Get two points, a and b, where a is the origin and b is very close to the origin node.
	// The slope of the two will give us the angle
	Vector2D a = Vector2D(navSplineX->getValue(0, 0.0), navSplineY->getValue(0, 0.0));
	Vector2D b = Vector2D(navSplineX->getValue(0, 0.1), navSplineY->getValue(0, 0.1));
	double angle = -Deg(std::atan2(b.x() - a.x(), b.y() - a.y()));
	return angle;
}

Vector2D SplineNavigation::getWheelSpeed(int tick, int interval) {

	bool rebuildWheelSplines = false;
	int actualInterval = interval;

	// Get the node based on the tick, and the current offset, so that you can start navigation in the middle or so
	int node = tickOffset + tick;
	double t = 0.0;

	// Register the currently used nav node and see if we have entered a new one.
	// Make sure it really is a new new node. Sometimes if the spline is 'corrupt'
	// (ie the curves are too tight and collapse into themselves) this new node
	// will be actually less than but not equal to the current...
	int newNavNode = getNavNodeForTick(tick);
	if(newNavNode > currentNavPoint) {

		currentNavPoint = newNavNode;
		//Debug::print("[Navigator] New Nav-Node %1", currentNavNode);

		// Set the current nav point in the spline to the actual position, so that we do not drift from the spline...
		if(dynamicSplineNodeAdjust == true) {
			navSplineX->changeNode(currentNavPoint, core->tracker->getX());
			navSplineY->changeNode(currentNavPoint, core->tracker->getY());
			rebuildWheelSplines = true;
		}
	}

	//Are we done?
	if (node >= wheelLeftSplineX->getNodeCount() - 1) {
		//Debug::print("[Navigation] Done at node %1 of %2", node, wheelLeftSplineX->getNodeCount());
		currentNavPoint++;
		return Vector2D(0.0,0.0);
	}

	// Dynamic or fixed interval?
	if(dynamicIntervalEnabled == true) {

		// Dynamic interval - adjust spline for incomming interval but only rebuild if exceeds flexibility
		actualInterval = lastGetWheelSpeedTimestamp.restart();

		// Rebuild spline?
		if(actualInterval > interval + intervalFlexibility || actualInterval < interval - intervalFlexibility) {
			//Debug::print("[Navigation] Interval flexiblity exceeded by %1", interval - actualInterval);
			rebuildWheelSplines = true;
		}

	} else {

		// Fixed interval, nothing to do except warning and abort if different
		if (this->interval != interval) {
			Debug::warning("[Navigation] The remote interval (%1) does not match the local interval (%2)!", interval, this->interval);
			return Vector2D(0.0,0.0);
		}
	}

	// Rebuild wheel splines because of interval delay or node offset?
	if(rebuildWheelSplines == true) {
		interval = actualInterval;
		calculateLeftRightWheelSpline();
	}

	// Calculate speed in mm/s per wheel
	double vl = norm(Vector2D(wheelLeftSplineX->getFirstDerivative(node, t), wheelLeftSplineY->getFirstDerivative(node, t)));
	double vr = norm(Vector2D(wheelRightSplineX->getFirstDerivative(node, t), wheelRightSplineY->getFirstDerivative(node, t)));
	vl *= 1.0 / (interval / 1000.0);
	vr *= 1.0 / (interval / 1000.0);

	// These are our delta vl and delta vr - what we modify on the velocity to
	// make up for error in the navigation
	double dvl = 0;
	double dvr = 0;

	// Get the transformation matrixes of our 'real' position and the position
	// we should be on the spline. From these we calculate the correction matrix
	// needed to undo the error. Based the correction angle we peer to the left
	// or to the right...
	if(magnetTrackingEnabled) {
		Trafo2D wheels = getWheelTransformation(node,t);
		Trafo2D robot = core->tracker->getTransformation();
		Trafo2D correction = wheels * robot.inv();
		//core->markerObject->setAsTransformation(0, robot); // debugging...
		//core->markerObject->setAsTransformation(1, wheels); // debugging...

		// Peer to the left or right based on the correction angle. The
		// amount we peer is directly related to the size of the angle, so
		// if the angles are very different, we will peer much faster to correct
		// our angle...
		double peerStrength = 150.0;
		double angle = Deg(correction.angle());
		if(angle > 0) {
			dvl = -peerStrength * angle/180; // Tune down the angle so that we have a better grip on fine-tuning peerstrength (150/180 is a small number to work with...)
			dvr = peerStrength * angle/180;
		} else {
			dvl = peerStrength * -angle/180;
			dvr = -peerStrength * -angle/180;
		}
	}

	// Return the speed of each wheel independently as a vector. This should be
	// passed to a Controller which sends the commands to the servos...
	return Vector2D(vl+dvl, vr+dvr);
}

void SplineNavigation::setTickOffset(int tick) {
	tickOffset = tick;
}


int SplineNavigation::getNavNodeForTick(int tick) {

	// Init
	double wheelNodeDistanceBetweenNode = targetSpeed * ((double) interval / 1000.0);
	int tickCount = 0;

	// Calculate backwards based on calculateLeftRightWheelSpline()...
	for (int navNode = 0; navNode < navSplineX->getNodeCount() - 1; navNode++) {
		double distanceBetweenNodes = getDistanceBetweenNavNodes(navNode, navNode+1);
		for (int wheelNode = 0; wheelNode < (distanceBetweenNodes / wheelNodeDistanceBetweenNode); wheelNode++) {
			tickCount++;
			if(tickCount >= tick) return navNode;
		}
	}
	return NavNodeNotFound;
}


double SplineNavigation::getDistanceBetweenNavNodes(int node1, int node2) {
	// Old: Distance based on geometry between the two nodes
	//Vector2D nodePosition1 = Vector2D(navSplineX->getValue(node1, 0), navSplineY->getValue(node1, 0));
	//Vector2D nodePosition2 = Vector2D(navSplineX->getValue(node2, 0), navSplineY->getValue(node2, 0));
	//return norm(nodePosition1 - nodePosition2);

	// Better attempt: interpolated distance based on geometry of each occuring node in between interpolated at 1/accuracy.
	double dist = 0;
	for(int node = node1; node < node2; node++) {
		dist += getCurveLengthOfNode(node, 100.0);
	}
	return dist;
}

double SplineNavigation::getCurveLengthOfNode(int node, double interpolations) {
	// TODO: This is not entirely accurate, but at close nodes quite accurate. Is there double inaccuracy?

	// Interpolated distance based at interval 1/inc:
	double dist = 0;
	double inc = 1.0 / interpolations;
	for(double t = 0; t < 1.0; t=t+inc) {
		Vector2D pos1 = Vector2D(navSplineX->getValue(node, t), navSplineY->getValue(node, t));
		Vector2D pos2 = Vector2D(navSplineX->getValue(node, t+inc), navSplineY->getValue(node, t+inc));
		dist += norm(pos1 - pos2);
	}
	return dist;
}

// Returns a vector which represents the displacement (difference)
// between the wheelSpline node position and the actual wheel position.
Vector2D SplineNavigation::getLeftWheelDisplacement(int node, double t) {
	return Vector2D(wheelLeftSplineX->getValue(node, t) - core->tracker->getWheelLeftTransformation().trans().x(), wheelLeftSplineY->getValue(node, t) - core->tracker->getWheelLeftTransformation().trans().y());
}

// Returns a vector which represents the displacement (difference)
// between the wheelSpline node position and the actual wheel position.
Vector2D SplineNavigation::getRightWheelDisplacement(int node, double t) {
	return Vector2D(wheelRightSplineX->getValue(node, t) - core->tracker->getWheelRightTransformation().trans().x(), wheelRightSplineY->getValue(node, t) - core->tracker->getWheelRightTransformation().trans().y());
}

// Returns a transformation matrix which represents the
// translation and rotation for the given node at t based
// on the left and right wheelSpline...
Trafo2D SplineNavigation::getWheelTransformation(int node, double t) {
	Vector2D center = Vector2D(wheelLeftSplineX->getValue(node,t) + wheelRightSplineX->getValue(node,t), wheelLeftSplineY->getValue(node,t) + wheelRightSplineY->getValue(node,t)) / 2; // Center point between left and right spline
	double angle = -std::atan2(wheelRightSplineX->getValue(node,t) - wheelLeftSplineX->getValue(node,t), wheelRightSplineY->getValue(node,t) - wheelLeftSplineY->getValue(node,t)) + Rad(90); // Angle between left and right (-90) in rad
	return Trafo2D::trans(center.x(), center.y()) * Trafo2D::rot(angle);
}

//void SplineNavigation::addNavPoint(Vector2D point) {
//	Navigation::addNavPoint(point);
//
//	// Append points to our datastructures
//	navSplineX->addNode(point.x());
//	navSplineY->addNode(point.y());
//
//	// Calculate points for wheel splines...
//	calculateLeftRightWheelSpline();
//}
//
//void SplineNavigation::addNavPoint(long x, long y) {
//	addNavPoint(Vector2D((double) x, (double) y)); // Forward to master function
//}

void SplineNavigation::processAddNavPoint(Vector2D point) {
	Navigation::processAddNavPoint(point);

	// Append points to our spline datastructures
	navSplineX->addNode(point.x());
	navSplineY->addNode(point.y());

	// Calculate points for wheel splines...
	calculateLeftRightWheelSpline();
}

void SplineNavigation::clearNavPoints() {
	Navigation::clearNavPoints();
	navSplineX->clearNodes();
	navSplineY->clearNodes();
	wheelLeftSplineX->clearNodes();
	wheelLeftSplineY->clearNodes();
	wheelRightSplineX->clearNodes();
	wheelRightSplineY->clearNodes();
	tickOffset = 0;
}
