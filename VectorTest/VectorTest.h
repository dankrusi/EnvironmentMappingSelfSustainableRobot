/*
 *  VectorTest.h
 *
 *  The Vector Test application (emssVectorTest) is a very simple program to
 *  visualize the rotation or movements of two vectors.
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

#ifndef VECTORTEST_H
#define VECTORTEST_H

#include <QWidget>
#include <QSlider>
#include <QLayout>
#include <QLabel>
#include <QFrame>

#include "Graph.h"

#include "vector2.h"
#include "trafo2.h"

class VectorTest : public QWidget
{
    Q_OBJECT

public:
    VectorTest(QWidget *parent = 0);
    ~VectorTest();

	Trafo2D t;

private:

	Graph *graph;
	QSlider *sliderX;
	QSlider *sliderY;
	QSlider *sliderRotation;
	QSlider *sliderScale;
	QFrame	*frame;

public slots:
	void updateFromSliders();
	void draw();
	void btnTx1();
	void btnTx2();
	void btnTy1();
	void btnTy2();
	void btnRot1();
	void btnRot2();

};

#endif // VECTORTEST_H
