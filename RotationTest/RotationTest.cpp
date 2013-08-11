/*
 *  RotationTest.cpp
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

#include "RotationTest.h"

#include <iostream>
#include <cmath>

#include <QPushButton>

RotationTest::RotationTest(QWidget *parent) : QWidget(parent)
{
	// init
	v1.x() = 0;
	v1.y() = 0;
	v2.x() = 0;
	v2.y() = 0;

	e.x() = 0;
	e.y() = 10;


	//set up GUI
	this->setLayout(new QVBoxLayout());
	this->layout()->setContentsMargins(0,0,0,0);
	this->setWindowTitle("Rotation Test");
	frame = new QFrame();
	QGridLayout *frameLayout = new QGridLayout();
	frame->setLayout(frameLayout);

	graph = new Graph();

	lblDist = new QLabel("Distance");
	frameLayout->addWidget(lblDist,0,0);
	lblDistValue = new QLabel("0");
	frameLayout->addWidget(lblDistValue,0,1);
	lblAngle = new QLabel("Angle");
	frameLayout->addWidget(lblAngle,1,0);
	lblAngleValue = new QLabel("0");
	frameLayout->addWidget(lblAngleValue,1,1);
	rbtnSource = new QRadioButton("Source");
	rbtnSource->setChecked(true);
	frameLayout->addWidget(rbtnSource,2,0);
	rbtnTarget = new QRadioButton("Target");
	frameLayout->addWidget(rbtnTarget,2,1);

	this->layout()->addWidget(graph);
	this->layout()->addWidget(frame);


	// connections
	connect(graph, SIGNAL(clicked(long, long)),this,SLOT(mouseClicked(long,long)));
	connect(graph, SIGNAL(released(long, long)),this,SLOT(mouseReleased(long,long)));
}

RotationTest::~RotationTest()
{
	delete graph;
}

void RotationTest::mouseClicked(long x, long y){
	if(rbtnSource->isChecked()){
		v1.x() = x;
		v1.y() = y;
	}
	else{
		v2.x() = x;
		v2.y() = y;
	}

	calculate();

	graph->draw(v1, v2, a1 * e, a2*e);
}

void RotationTest::mouseReleased(long x, long y){

}

/*
 *
 *
 *      /
 *     /
 *    /
 *   /
 *  /
 *
 */
void RotationTest::calculate(){


	a1 = a1.trans(v1.x(), v1.y()) * a1.rot(Rad(0));
	a2 = a1.trans(v2.x(), v2.y()) * a2.rot(Rad(0));

	Trafo2L a = a2 * a1.inv();

	//distance
	double distance = std::sqrt(a.trans().x()*a.trans().x() + a.trans().y()*a.trans().y());
	QString s1;
	lblDistValue->setText(s1.setNum(distance));

	//angle
	double angle = std::atan2(a.trans().x(), a.trans().y() );

	QString s2;
	lblAngleValue->setText(s2.setNum(Deg(angle)));

	a2 = a2.rot(angle);

	a2.print();

}


