/*
 *  DifferentialSteeringTest.cpp
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

#import "DifferentialSteeringTest.h"

#import "Graph.h"

#include "diffsteer2.h"

DifferentialSteeringTest::DifferentialSteeringTest(QWidget *parent) :
	QWidget(parent) {

	this->setWindowTitle("DifferentialSteeringTest");
	this->setLayout(new QVBoxLayout());
	this->layout()->setContentsMargins(0, 0, 0, 0);

	graph = new Graph();
	slider = new QSlider(Qt::Horizontal);
	slider->setValue(50);
	btnCalculate = new QPushButton("Clear");

	this->layout()->addWidget(graph);
	this->layout()->addWidget(slider);
	this->layout()->addWidget(btnCalculate);

	connect(btnCalculate, SIGNAL(clicked()), this, SLOT(clear()));
	connect(graph, SIGNAL(clicked(double, double)), this, SLOT(clicked(double,double)));
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));

	getResults();
}

DifferentialSteeringTest::~DifferentialSteeringTest() {

}

void DifferentialSteeringTest::clicked(double x, double y) {

	getResults();
	graph->setResults(results);

}

void DifferentialSteeringTest::clear() {

	getResults();
	graph->setResults(results);
}


void DifferentialSteeringTest::getResults() {

	// Init
	results.clear();

	real phi = M_PI * -10 / 180.0;
	real x0 = 0;
	diffeq y0 = { { 2, 1, cos (phi), sin (phi), -sin (phi), cos (phi) } };
	real h = 0.1;
	real xmax = 20;

	for (;;)
	{
		results.push_back(Vector2D(y0.coord[0], y0.coord[1]));

		//printf("%f %f \n", y0.coord[0], y0.coord[1]);

		if (x0 >= xmax) break;

		y0 = RungeKutta (x0, y0, h);
		x0 += h;
	}

}


void DifferentialSteeringTest::sliderChanged(int value) {
	graph->setScale(value);
	getResults();
	graph->setResults(results);
}
