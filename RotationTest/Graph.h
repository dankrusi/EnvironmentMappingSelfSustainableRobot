/*
 *  Graph.h
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

#ifndef GRAPH_H_
#define GRAPH_H_

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

#include "../Core/Library/Math/vector2.h"
#include "../Core/Library/Math/trafo2.h"

class Graph : public QWidget {

	Q_OBJECT

private:

	Vector2T<long> v1;
	Vector2T<long> v2;
	Vector2T<long> e1;
	Vector2T<long> e2;

	Trafo2T<double> robot;

public:
	Graph(QWidget *parent = 0);
	virtual ~Graph();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent (QMouseEvent *event);

public slots:
	void paint();
	void draw( Vector2T<long> v1, Vector2T<long> v2, Vector2T<long> e1, Vector2T<long> e2);

signals:
	void clicked(long x, long y);
	void released(long x, long y);

};

#endif /* GRAPH_H_ */
