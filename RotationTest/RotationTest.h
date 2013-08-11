/*
 *  RotationTest.h
 *
 *  The Rotation Test application (emssRotationTest) was built to help us
 *  to understand the concept of a rotation matrix. It is a very simple
 *  program with a modest GUI. 	This application holds two transformation
 *  matrices A1 and A2, and outputs the difference of angle and the distance
 *  between them.
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

#ifndef ROTATIONTEST_H
#define ROTATIONTEST_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QRadioButton>

#include "../Core/Library/Math/vector2.h"
#include "../Core/Library/Math/trafo2.h"
#include "../Core/Library/Math/math-ext.h"
#include "Graph.h"

class RotationTest : public QWidget
{
    Q_OBJECT

private:
	Graph *graph;
	QRadioButton *rbtnSource;
	QRadioButton *rbtnTarget;
	QFrame *frame;
	QLabel *lblDist;
	QLabel *lblDistValue;
	QLabel *lblAngle;
	QLabel *lblAngleValue;
	Vector2L v1;
	Vector2L v2;
	Vector2L e;
	Trafo2L a1;
	Trafo2L a2;

public:
	RotationTest(QWidget *parent = 0);
    ~RotationTest();
    void calculate();

	Trafo2D t;

public slots:
	void mouseClicked(long x, long y);
	void mouseReleased(long x, long y);

};

#endif // ROTATIONTEST_H
