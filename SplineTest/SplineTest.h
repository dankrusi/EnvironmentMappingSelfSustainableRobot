/*
 *  SplineTest.h
 *
 *  To test the different splines we implemented the emssSplineTest application.
 *  Various navigation points can be set, which in turn calculates the curve
 *  of the spline. Additional information such as the first derivative can be
 *  output as well.
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

#ifndef SPLINETEST_H_
#define SPLINETEST_H_

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QDoubleSpinBox>

#include <iostream>

#include "Graph.h"

#include "vector2.h"
#include "spline1.h"

//asfdsd

class SplineTest : public QWidget
{
	Q_OBJECT


public:
	Graph *graph;
	QComboBox *cbKernel;
	QPushButton *btnCalculate;
	QDoubleSpinBox *dblSplinedy0;
	QDoubleSpinBox *dblSplinedyn;
	QDoubleSpinBox *dblSplineXdy0;
	QDoubleSpinBox *dblSplineXdyn;
	QDoubleSpinBox *dblSplineYdy0;
	QDoubleSpinBox *dblSplineYdyn;

private:

	std::vector<Vector2D> points;
	std::vector<Vector2D> results;
	std::vector<double> tangents;

	Spline1D splineX;
	Spline1D splineY;


public:
	SplineTest(QWidget *parent = 0);
	~SplineTest();
	void getResults();


public slots:
	void clear();
	void constraintsChanged();
	void constraintsChangedXY();
	void kernelChanged();
	void clicked(double, double);

};


#endif /* SPLINETEST_H_ */
