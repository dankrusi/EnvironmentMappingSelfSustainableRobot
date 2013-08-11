/*
 * Tracker.h
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

#ifndef TRACKER_H_
#define TRACKER_H_

#include <QList>
#include <QReadWriteLock>

#include "../CoreObject.h"
#include "MovementTracker.h"

class Tracker : public CoreObject {

	Q_OBJECT

protected:
	QList<MovementTracker* > *trackers;
	QReadWriteLock lock; // Lock used when accessing trackers. Also to be used by subclasses...

public:
	Tracker(QString name, Core *core);
	~Tracker();
	void connectController(QObject *target);
	void addMovementTracker(MovementTracker* movementTracker);
	long getX(); // X coordinate in milli-meters
	long getY(); // Y coordinate in milli-meters
	double getRotation();
	Vector2D getTranslation();
	Trafo2D getWheelLeftTransformation();
	Trafo2D getWheelRightTransformation();
	void setTransformation(Trafo2D t);

public:
	virtual Trafo2D getTransformation() = 0;
	virtual long getTotalDistance() = 0;
	virtual long getTotalAngle() = 0;

public slots:
	/* Slots for movement signals from Controller or other interfaces */
	void registerMovedDistance(double distance);
	void registerChangedAngle(double angle);
	//void registerObjectDetected(double distance, double angle, double opacity, int size);
	void registerChangedWheelSpeed(int, int);

signals:
	void moved(long x, long y, double rotation);
};

#endif /* TRACKER_H_ */
