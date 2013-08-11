/*
 *  Navigation.h
 *
 *  The Navigation class holds the information of the navigation points,
 *  or way points, for the robot. Other data structures, such as splines,
 *  are also included in this class. Tasks who wish to navigate from point
 *  to point or via spline use the Navigation component as an abstraction
 *  to do so. The most important methods are calculateLeftRightWheelSpline()
 *  and getWheelSpeed(tick,interval)
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

#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include <QList>
#include <QTime>

#include "../CoreObject.h"
#include "../Library/Math.h"
#include "../Library/Math/trafo2.h"

class Navigation : public CoreObject {

	Q_OBJECT;


public:
	enum CollisionDirection { CollisionDirectionAhead, CollisionDirectionBackwards };
	enum CollisionSource { CollisionSourceHeatMap, CollisionSourceFadingCollisionMap };

protected:
	QList<Vector2D> *navPoints;	// The navigation points which the robot must drive along
	QList<Vector2D> *navPath;	// The physical navigation path which the robot drove

public:
	int currentNavPoint;		// The last nav node which corresponds to the wheelnode tick, it is calculated backwards...

public:
	Navigation(QString name, Core *core);
	virtual ~Navigation();
	virtual void addNavPoint(Vector2D point);
	virtual void addNavPoint(long x, long y);
	void registerPointOnNavPath(Vector2D point);
	void registerPointOnNavPath(long x, long y);
	QList<Vector2D>* getNavPoints();
	QList<Vector2D>* getNavPath();
	virtual void clearNavPoints();
	virtual Vector2D getWheelSpeed(int tick, int interval) = 0;
	virtual int getCurrentNavPoint();
	virtual bool isAtLastNavPoint();
	virtual Vector2D getCurrentDestination();
	long getMaxOpenAreaDistanceFromMap(CollisionSource source, CollisionDirection direction);
	int getWallDistanceFromMap(CollisionSource source);

protected:
	virtual void processAddNavPoint(Vector2D point);

};

#endif /* NAVIGATION_H_ */
