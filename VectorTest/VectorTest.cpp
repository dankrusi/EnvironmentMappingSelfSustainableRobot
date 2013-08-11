/*
 *  VectorTest.cpp
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

#include "VectorTest.h"

#include <iostream>

#include <QPushButton>


VectorTest::VectorTest(QWidget *parent) : QWidget(parent)
{
	this->setLayout(new QVBoxLayout());
	this->layout()->setContentsMargins(0,0,0,0);
	this->setWindowTitle("Vector Test");

	graph = new Graph();

	frame = new QFrame();
	QGridLayout *frameLayout = new QGridLayout();
	frame->setLayout(frameLayout);

	sliderX = new QSlider(Qt::Horizontal);
	sliderX->setRange(0,10);
	sliderY = new QSlider(Qt::Horizontal);
	sliderY->setRange(0,10);
	sliderRotation = new QSlider(Qt::Horizontal);
	sliderRotation->setRange(0,360);
	sliderScale = new QSlider(Qt::Horizontal);
	sliderScale->setRange(1,100);

	this->layout()->addWidget(graph);
	this->layout()->addWidget(frame);

	frameLayout->addWidget(new QLabel("X-Position"), 0, 0);
	frameLayout->addWidget(sliderX, 0, 1);
	frameLayout->addWidget(new QLabel("Y-Position"), 1, 0);
	frameLayout->addWidget(sliderY, 1, 1);
	frameLayout->addWidget(new QLabel("Rotation"), 2 , 0);
	frameLayout->addWidget(sliderRotation, 2, 1);
	frameLayout->addWidget(new QLabel("Scale"), 3, 0);
	frameLayout->addWidget(sliderScale, 3, 1);


	QPushButton *btnTx1 = new QPushButton("Tx Add");
	frameLayout->addWidget(btnTx1, 4, 1);
	connect(btnTx1, SIGNAL(clicked()), this, SLOT(btnTx1()));
	QPushButton *btnTx2 = new QPushButton("Tx Subtract");
	frameLayout->addWidget(btnTx2, 4, 0);
	connect(btnTx2, SIGNAL(clicked()), this, SLOT(btnTx2()));

	QPushButton *btnTy1 = new QPushButton("Ty Add");
	frameLayout->addWidget(btnTy1, 5, 0);
	connect(btnTy1, SIGNAL(clicked()), this, SLOT(btnTy1()));
	QPushButton *btnTy2 = new QPushButton("Ty Subtract");
	frameLayout->addWidget(btnTy2, 5, 1);
	connect(btnTy2, SIGNAL(clicked()), this, SLOT(btnTy2()));

	QPushButton *btnRot1 = new QPushButton("Rot Add");
	frameLayout->addWidget(btnRot1, 6, 0);
	connect(btnRot1, SIGNAL(clicked()), this, SLOT(btnRot1()));
	QPushButton *btnRot2 = new QPushButton("Rot Subtract");
	frameLayout->addWidget(btnRot2, 6, 1);
	connect(btnRot2, SIGNAL(clicked()), this, SLOT(btnRot2()));

	connect(sliderX, SIGNAL(valueChanged(int)), this, SLOT(updateFromSliders()));
	connect(sliderY, SIGNAL(valueChanged(int)), this, SLOT(updateFromSliders()));
	connect(sliderRotation, SIGNAL(valueChanged(int)), this, SLOT(updateFromSliders()));
	connect(sliderScale, SIGNAL(valueChanged(int)), this, SLOT(updateFromSliders()));

	updateFromSliders();
}

VectorTest::~VectorTest()
{

}

void VectorTest::btnTx1() {
	// Tx Add
	t = t*t.trans(1,0);
	draw();
}
void VectorTest::btnTx2() {
	// Tx Subtract
	t = t*t.trans(-1,0);
	draw();
}

void VectorTest::btnTy1() {
	// Ty Add
	t = t*t.trans(0,1);
	draw();
}
void VectorTest::btnTy2() {
	// Ty Subtract
	t = t*t.trans(0,-1);
	draw();
}

void VectorTest::btnRot1() {
	// Rot Add
	t = t*t.rot(Rad(20));
	draw();
}
void VectorTest::btnRot2() {
	// Rot Subtract
	t = t*t.rot(Rad(-20));
	draw();
}

void VectorTest::updateFromSliders(){

	// Calculate
	Vector2D p(sliderX->value(), sliderY->value());
	//Trafo2D t = Trafo2D::trans(sliderX->value(), sliderY->value()) * Trafo2D::rot(Rad(sliderRotation->value())) ;
	t = Trafo2D::rot(Rad(sliderRotation->value())) * Trafo2D::trans(sliderX->value(), sliderY->value());
	//Trafo2D t = Trafo2D::trans(sliderX->value(), sliderY->value());
	//Trafo2D t = Trafo2D::rot(Rad(sliderRotation->value())) ;

//	for (int j = 0; j < 3; j++){
//		for (int k = 0; k < 3; k++){
//			std::cout << " " << t.Data[j][k];
//		}
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//


	//p = Trafo2D::rot(30) * p;
	//r.rot(20);
	//p = r*p;


	draw();

}

void VectorTest::draw(){

	Vector2D u = t * Vector2D(0,0);
	Vector2D v = t * Vector2D(1,0);
	Vector2D w = t * Vector2D(0,1);
	graph->draw(u.x(), u.y(), v.x(), v.y(), w.x(), w.y(), sliderScale->value());

}
