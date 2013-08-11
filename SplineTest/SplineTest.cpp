/*
 *  SplineTest.cpp
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

#import "SplineTest.h"

#import "Graph.h"

SplineTest::SplineTest(QWidget *parent) :
	QWidget(parent) {

	this->setWindowTitle("SplineTest");
	this->setLayout(new QVBoxLayout());

	this->layout()->setContentsMargins(0, 0, 0, 0);

	cbKernel = new QComboBox();
	cbKernel->addItem("Bruenner");
	cbKernel->addItem("NRNatural");
	cbKernel->addItem("Stoer");
	cbKernel->addItem("Wirth");

	btnCalculate = new QPushButton("Clear");

	graph = new Graph();

	QWidget *dxdyPanel1 = new QWidget();
	dxdyPanel1->setLayout(new QHBoxLayout());
	dblSplinedy0 = new QDoubleSpinBox();
	dblSplinedy0->setSingleStep(10);
	dblSplinedy0->setValue(0.0);
	dblSplinedy0->setToolTip("Spline X and Y dy0");
	dblSplinedy0->setMinimum(-100000.0);
	dblSplinedy0->setMaximum(100000.0);
	dblSplinedyn = new QDoubleSpinBox();
	dblSplinedyn->setSingleStep(10);
	dblSplinedyn->setValue(0.0);
	dblSplinedyn->setToolTip("Spline X and Y dyn");
	dblSplinedyn->setMinimum(-100000.0);
	dblSplinedyn->setMaximum(100000.0);
	dxdyPanel1->layout()->addWidget(dblSplinedy0);
	dxdyPanel1->layout()->addWidget(dblSplinedyn);

	QWidget *dxdyPanel2 = new QWidget();
	dxdyPanel2->setLayout(new QHBoxLayout());
	dblSplineXdy0 = new QDoubleSpinBox();
	dblSplineXdy0->setSingleStep(0.5);
	dblSplineXdy0->setValue(0.0);
	dblSplineXdy0->setToolTip("Spline X dy0");
	dblSplineXdy0->setMinimum(-100000.0);
	dblSplineXdy0->setMaximum(100000.0);
	dblSplineXdyn = new QDoubleSpinBox();
	dblSplineXdyn->setSingleStep(0.5);
	dblSplineXdyn->setValue(0.0);
	dblSplineXdyn->setToolTip("Spline X dyn");
	dblSplineXdyn->setMinimum(-100000.0);
	dblSplineXdyn->setMaximum(100000.0);
	dblSplineYdy0 = new QDoubleSpinBox();
	dblSplineYdy0->setSingleStep(0.5);
	dblSplineYdy0->setValue(0.0);
	dblSplineYdy0->setToolTip("Spline Y dy0");
	dblSplineYdy0->setMinimum(-100000.0);
	dblSplineYdy0->setMaximum(100000.0);
	dblSplineYdyn = new QDoubleSpinBox();
	dblSplineYdyn->setSingleStep(0.5);
	dblSplineYdyn->setValue(0.0);
	dblSplineYdyn->setToolTip("Spline Y dyn");
	dblSplineYdyn->setMinimum(-100000.0);
	dblSplineYdyn->setMaximum(100000.0);
	dxdyPanel2->layout()->addWidget(dblSplineXdy0);
	dxdyPanel2->layout()->addWidget(dblSplineYdy0);
	dxdyPanel2->layout()->addWidget(dblSplineXdyn);
	dxdyPanel2->layout()->addWidget(dblSplineYdyn);

	this->layout()->addWidget(graph);
	this->layout()->addWidget(cbKernel);
	this->layout()->addWidget(dxdyPanel1);
	this->layout()->addWidget(dxdyPanel2);
	this->layout()->addWidget(btnCalculate);

	connect(cbKernel, SIGNAL(currentIndexChanged(int)), this, SLOT(kernelChanged()));
	connect(btnCalculate, SIGNAL(clicked()), this, SLOT(clear()));
	connect(dblSplinedy0, SIGNAL(valueChanged(double)), this, SLOT(constraintsChanged()));
	connect(dblSplinedyn, SIGNAL(valueChanged(double)), this, SLOT(constraintsChanged()));
	connect(dblSplineXdy0, SIGNAL(valueChanged(double)), this, SLOT(constraintsChangedXY()));
	connect(dblSplineXdyn, SIGNAL(valueChanged(double)), this, SLOT(constraintsChangedXY()));
	connect(dblSplineYdy0, SIGNAL(valueChanged(double)), this, SLOT(constraintsChangedXY()));
	connect(dblSplineYdyn, SIGNAL(valueChanged(double)), this, SLOT(constraintsChangedXY()));
	connect(graph, SIGNAL(clicked(double, double)), this, SLOT(clicked(double,double)));
}

SplineTest::~SplineTest() {

}

void SplineTest::clicked(double x, double y) {

	points.push_back(Vector2D(x,y));

	splineX.addNode(x);
	splineY.addNode(y);

	getResults();
	graph->setResults(points, results, tangents);

}

void SplineTest::clear(){
	points.clear();

	splineX.clearNodes();
	splineY.clearNodes();

	getResults();
	graph->setResults(points, results, tangents);
}

void SplineTest::kernelChanged(){
	if     (cbKernel->currentIndex() == 0) 	{
		splineX.setKernel(SplineKernelBruenner);
		splineY.setKernel(SplineKernelBruenner);
	} else if(cbKernel->currentIndex() == 1) 	{
		splineX.setKernel(SplineKernelNRNatural);
		splineY.setKernel(SplineKernelNRNatural);
	} else if(cbKernel->currentIndex() == 2) 	{
		splineX.setKernel(SplineKernelStoer);
		splineY.setKernel(SplineKernelStoer);
	} else if(cbKernel->currentIndex() == 3) 	{
		splineX.setKernel(SplineKernelWirth);
		splineY.setKernel(SplineKernelWirth);
	}
	getResults();
	graph->setResults(points, results, tangents);
}

void SplineTest::constraintsChanged(){
	dblSplineXdy0->setValue(dblSplinedy0->value());
	dblSplineXdyn->setValue(dblSplinedyn->value());
	dblSplineYdy0->setValue(dblSplinedy0->value());
	dblSplineYdyn->setValue(dblSplinedyn->value());
	constraintsChangedXY();
}

void SplineTest::constraintsChangedXY(){
	double v = 1000;
	splineX.setConstraints(v*cosf(Rad(dblSplineXdy0->value())), v*sinf(Rad(dblSplineXdyn->value())));
	splineY.setConstraints((dblSplineYdy0->value()), (dblSplineYdyn->value()));
	getResults();
	graph->setResults(points, results, tangents);
}

void SplineTest::getResults() {

	// Init
	results.clear();
	tangents.clear();

	if(points.size() == 0) return;

	for (uint i = 0; i < points.size() - 1; i++) {
		for (uint t = 0; t < 30; t++) {
			double px = splineX.getValue(i, ((double) t) / 30.0);
			double py = splineY.getValue(i, ((double) t) / 30.0);
			results.push_back(Vector2D(px, py));
//			if(t == 0) printf("SplineX: x=%d\ty=%f\ty'=%f\n",i,splineX.getValue(i,0.0),splineX.getFirstDerivative(i,0.0));
//			if(t == 0) printf("SplineY: x=%d\ty=%f\ty'=%f\n",i,splineY.getValue(i,0.0),splineY.getFirstDerivative(i,0.0));
		}
	}

	for (uint i = 0; i < points.size(); i++) {
		double dydx = 3.14;
		if(i == points.size()-1) {
			if(splineX.getFirstDerivative(i-1, 1.0) != 0.0) dydx = splineY.getFirstDerivative(i-1, 1.0) / splineX.getFirstDerivative(i-1, 1.0);
		} else {
			if(splineX.getFirstDerivative(i, 0) != 0.0) dydx = splineY.getFirstDerivative(i, 0) / splineX.getFirstDerivative(i, 0);
		}
		tangents.push_back(dydx);
	}

	// Console output
	printf("-----------------------------------\n\n");
	for (uint i = 0; i < points.size(); i++) {
		printf("SplineX: x=%d\ty=%f\ty'=%f\ty''=%f\n",i,splineX.getValue(i,0.0),splineX.getFirstDerivative(i,0.0),splineX.getSecondDerivative(i,0.0));
		printf("SplineY: x=%d\ty=%f\ty'=%f\ty''=%f\n",i,splineY.getValue(i,0.0),splineY.getFirstDerivative(i,0.0),splineY.getSecondDerivative(i,0.0));
	}
	printf("splineX := { ");
	for (uint i = 0; i < points.size(); i++) {

		printf("{%d,%f,%f,%f}",i,splineX.getValue(i,0.0),splineX.getFirstDerivative(i,0.0),splineX.getSecondDerivative(i,0.0));
		if(i != points.size()-1) printf(",");
	}
	printf(" };\n");
	printf("splineY := { ");
	for (uint i = 0; i < points.size(); i++) {

		printf("{%d,%f,%f,%f}",i,splineY.getValue(i,0.0),splineY.getFirstDerivative(i,0.0),splineY.getSecondDerivative(i,0.0));
		if(i != points.size()-1) printf(",");
	}
	printf(" };\n");
}
