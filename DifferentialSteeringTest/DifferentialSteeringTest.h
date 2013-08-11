/*
 *  DifferentialSteeringTest.h
 *
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

#ifndef DIFFERENTIALSTEERINGTEST_H_
#define DIFFERENTIALSTEERINGTEST_H_

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSlider>
#include <QDoubleSpinBox>

#include "Graph.h"

#include <vector>
#include "vector2.h"

class DifferentialSteeringTest : public QWidget
{
	Q_OBJECT


public:
	Graph *graph;
	QSlider *slider;
	QPushButton *btnCalculate;

private:

	std::vector<Vector2D> results;


public:
	DifferentialSteeringTest(QWidget *parent = 0);
	~DifferentialSteeringTest();
	void getResults();


public slots:
	void clear();
	void clicked(double, double);
	void sliderChanged(int value);

};


#endif /* DIFFERENTIALSTEERINGTEST_H_ */
