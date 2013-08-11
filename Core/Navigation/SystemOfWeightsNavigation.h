/*
 *  SystemOfWeightsNavigation.h
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

#ifndef SYSTEMOFWEIGHTSNAVIGATION_H_
#define SYSTEMOFWEIGHTSNAVIGATION_H_

#include <QList>
#include <QReadWriteLock>

#include "Navigation.h"

class Weight;

class SystemOfWeightsNavigation : public Navigation {

	Q_OBJECT;

private:
	QList<Weight*> *weights;	// Ordered list of weights which build up the virtual formula for the left and right wheel channel
	QReadWriteLock lock;
	bool stopAtLastNavPoint;

public:
	SystemOfWeightsNavigation(Core *core, bool createDefaultWeights);
	virtual ~SystemOfWeightsNavigation();
	virtual Vector2D getWheelSpeed(int tick, int interval);
	const QList<Weight*> *getWeights();
	void removeAllWeights();
	void addWeight(Weight* weight);
	bool removeWeight(QString name);
	bool removeWeight(Weight* weight);
	void moveWeightDown(Weight* weight);
	void moveWeightUp(Weight* weight);

signals:
	void weightsChanged();

};

#endif /* SYSTEMOFWEIGHTSNAVIGATION_H_ */
