/*
 *  MedialAxisTest.cpp
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

#import "MedialAxisTest.h"

#import "Graph.h"
#import "BinaryPriorityQueue.h"
#import "vector2.h"

MedialAxisTest::MedialAxisTest(QWidget *parent) :
	QWidget(parent) {

	this->setWindowTitle("MedialAxisTest");
	this->setLayout(new QVBoxLayout());

	this->layout()->setContentsMargins(0, 0, 0, 0);

	cbKernel = new QComboBox();
	cbKernel->addItem("Default");

	btnCalculate = new QPushButton("Clear");

	graph = new Graph();

	this->layout()->addWidget(graph);
	this->layout()->addWidget(cbKernel);
	this->layout()->addWidget(btnCalculate);

	connect(cbKernel, SIGNAL(currentIndexChanged(int)), this, SLOT(kernelChanged()));
	connect(btnCalculate, SIGNAL(clicked()), this, SLOT(clear()));
	connect(graph, SIGNAL(clicked(double, double)), this, SLOT(clicked(double,double)));
}

MedialAxisTest::~MedialAxisTest() {

}

void MedialAxisTest::clicked(double x, double y) {

	points.append(Vector2D(x,y));
//	points.push_back(Vector2D(x,y));
//
//	splineX.addNode(x);
//	splineY.addNode(y);
//
//	getResults();
	graph->setPoints(points);

}

void MedialAxisTest::clear(){
	points.clear();
//
//	splineX.clearNodes();
//	splineY.clearNodes();
//
//	getResults();
	graph->setPoints(points);
}

void MedialAxisTest::kernelChanged(){
//	if     (cbKernel->currentIndex() == 0) 	{
//		splineX.setKernel(SplineKernelBruenner);
//		splineY.setKernel(SplineKernelBruenner);
//	}
//	else if(cbKernel->currentIndex() == 1) 	{
//		splineX.setKernel(SplineKernelNRNatural);
//		splineY.setKernel(SplineKernelNRNatural);
//	}
//	else if(cbKernel->currentIndex() == 2) 	{
//		splineX.setKernel(SplineKernelStoer);
//		splineY.setKernel(SplineKernelStoer);
//	}
//	getResults();
//	graph->setResults(points, results, tangents);
}





void MedialAxisTest::getResults() {

//	// Init
//	results.clear();
//	tangents.clear();
//
//	if(points.size() == 0) return;
//
//	for (uint i = 0; i < points.size() - 1; i++) {
//		for (uint t = 0; t < 30; t++) {
//			double px = splineX.getValue(i, ((double) t) / 30.0);
//			double py = splineY.getValue(i, ((double) t) / 30.0);
//			results.push_back(Vector2D(px, py));
//			if(t == 0) printf("SplineX: x=%d\ty=%f\ty'=%f\n",i,splineX.getValue(i,0.0),splineX.getFirstDerivative(i,0.0));
//			if(t == 0) printf("SplineY: x=%d\ty=%f\ty'=%f\n",i,splineY.getValue(i,0.0),splineY.getFirstDerivative(i,0.0));
//		}
//	}
//
//	for (uint i = 0; i < points.size(); i++) {
//		double dydx = 3.14;
//		if(i == points.size()-1) {
//			if(splineX.getFirstDerivative(i-1, 1.0) != 0.0) dydx = splineY.getFirstDerivative(i-1, 1.0) / splineX.getFirstDerivative(i-1, 1.0);
//		} else {
//			if(splineX.getFirstDerivative(i, 0) != 0.0) dydx = splineY.getFirstDerivative(i, 0) / splineX.getFirstDerivative(i, 0);
//		}
//		tangents.push_back(dydx);
//	}
//
//	// Console output
//	printf("-----------------------------------\n\n");
//	for (uint i = 0; i < points.size(); i++) {
//		printf("SplineX: x=%d\ty=%f\ty'=%f\ty''=%f\n",i,splineX.getValue(i,0.0),splineX.getFirstDerivative(i,0.0),splineX.getSecondDerivative(i,0.0));
//		printf("SplineY: x=%d\ty=%f\ty'=%f\ty''=%f\n",i,splineY.getValue(i,0.0),splineY.getFirstDerivative(i,0.0),splineY.getSecondDerivative(i,0.0));
//	}
//	printf("splineX := { ");
//	for (uint i = 0; i < points.size(); i++) {
//
//		printf("{%d,%f,%f,%f}",i,splineX.getValue(i,0.0),splineX.getFirstDerivative(i,0.0),splineX.getSecondDerivative(i,0.0));
//		if(i != points.size()-1) printf(",");
//	}
//	printf(" };\n");
//	printf("splineY := { ");
//	for (uint i = 0; i < points.size(); i++) {
//
//		printf("{%d,%f,%f,%f}",i,splineY.getValue(i,0.0),splineY.getFirstDerivative(i,0.0),splineY.getSecondDerivative(i,0.0));
//		if(i != points.size()-1) printf(",");
//	}
//	printf(" };\n");
}
